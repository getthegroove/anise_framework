#include "nodemesh.h"
#include "nodefactory.h"
#include "nodestarttask.h"
#include "../../src_common/qt-json/json.h"
#include "../data/datafactory.h"
#include "../data/messagedata.h"
#include <QDebug>
#include <QThreadPool>


//------------------------------------------------------------------------------
// Constructor and Destructor

CNodeMesh::CNodeMesh()
    : m_nodes()
    , m_nodes_waiting(0)
    , m_start_success(true)
    , m_nodes_processing(0)
{

}


//------------------------------------------------------------------------------
// Public Functions

bool CNodeMesh::parseMesh(QString json_str)
{
    QVariant json_variant = QtJson::parse(json_str).toMap();

    if(json_variant.isNull()) {
        qWarning() << "CNodeMesh::parseMesh(): Failed to parse mesh file.";
        return false;
    }

    QVariantMap json_map = json_variant.toMap();

    // Create and add all the Nodes to our collection of nodes.
    for(QVariant variant : json_map["nodes"].toList()) {
        QVariantMap map = variant.toMap();
        addNode(map);
    }

    // Create the links between the nodes.
    for(QVariant variant : json_map["connections"].toList()) {
        QVariantMap conn = variant.toMap();
        addConnection(conn);
    }

    return true;
}

void CNodeMesh::startNodes()
{
    // Set how many nodes we are going to wait for.
    m_nodes_waiting = m_nodes.size();

    QMap<QString, QSharedPointer<CNode>>::iterator i;
    for(i = m_nodes.begin(); i != m_nodes.end(); ++i) {
        auto node = i.value();
        // Create a runnable task that will start the nodes in parallel.
        CNodeStartTask *start_task = new CNodeStartTask(*(node.data()));
        QObject::connect(start_task, SIGNAL(taskFinished(bool)),
                         this, SLOT(onNodeStarted(bool)));

        // Let the ThreadPool delete start_task after finishing.
        start_task->setAutoDelete(true);
        // Start the nodes in parallel using all available threads.
        QThreadPool::globalInstance()->start(start_task);
    }
}

void CNodeMesh::startSimulation()
{
    // Look for nodes that have no inputs.
    qint8 input_gates = 0;
    // Flag to keep track whether the simulation is started or not.
    bool simulation_started = false;

    // Create the message that will be sent.
    CMessageData *msg =
        static_cast<CMessageData *>(CDataFactory::instance().createData("message"));
    if(msg == nullptr) {
        qCritical() << "CNodeMesh::startSimulation(): Could not create start"
                   << "message.";
        return;
    }
    msg->setMessage("start");
    QSharedPointer<CData> pmsg = QSharedPointer<CData>(msg);

    // Look for nodes without input gates and send them pmsg.
    QMap<QString, QSharedPointer<CNode>>::iterator i;
    for(i = m_nodes.begin(); i != m_nodes.end(); ++i) {
        auto node = i.value();
        input_gates = node->inputGatesSize();
        if(input_gates == 0) {
            node->processData("", pmsg);
            simulation_started = true;
        }
    }

    if(!simulation_started) {
        qWarning() << "CNodeMesh::startSimulation(): The simulation was"
                   << "not started because we could not figure out where to start.";
        emit simulationFinished();
    }
}



//------------------------------------------------------------------------------
// Private Functions

bool CNodeMesh::addNode(QVariantMap &node_json)
{
    CNodeConfig conf;
    bool ok;
    QString node_name;
    QString node_class;
    QVariant v;

    v = node_json["name"];
    if(v.isValid()) {
        node_name = v.toString();
    }

    v = node_json["class"];
    if(v.isValid()) {
        node_class = v.toString();
    }

    // Verify that this Node was defined properly.
    if(node_name.isEmpty() || node_class.isEmpty()) {
        qWarning() << "CNodeMesh::addNode():"
                   << "The JSON Node definition did not include class or name.";
        return false;
    }

    // Verify that a Node with the same name does not exist already in the map.
    if(m_nodes.contains(node_name)) {
        qWarning() << "CNodeMesh::addNode():"
                   << "A node with the name" << node_name
                   << "has already been added to the mesh.";
        return false;
    }

    // Get the configuration template of the requested node.
    ok = CNodeFactory::instance().configTemplate(
        node_class, conf);
    if(!ok) {
        // The template for the desired Node class was not found.
        qWarning() << "CNodeMesh::addNode(): The node class"
                   << node_name << "failed to set its config template.";
        return false;
    }

    // Set the node name.
    conf.setName(node_name);

    // Set the node Parameters.
    for(QVariant p : node_json["params"].toList()) {
        QVariantMap param = p.toMap();
        for(QVariant key : param.keys()) {
            ok = conf.setParameter(key.toString(), param.value(key.toString()));
            if(!ok) {
                qWarning() << "CNodeMesh::addNode():"
                           << "Failed to set the parameter" << key.toString()
                           << "in Node" << node_name << ".";
                return false;
            }
        }
    }

    // Create the node we've been asked for.
    CNode *node = CNodeFactory::instance().createNode(node_class, conf);
    if(node == nullptr) {
        qWarning() << "CNodeMesh::addNode():"
                   << "Could not create Node" << node_class << ".";
        return false;
    }
    m_nodes.insert(node_name, QSharedPointer<CNode>(node));
    // Keep track the processing status of the node.
    QObject::connect(node, SIGNAL(processing(bool)),
                     this, SLOT(onNodeProcessing(bool)));

    return true;
}

bool CNodeMesh::addConnection(QVariantMap& connections_json)
{
    QSharedPointer<CNode> src_node;
    QSharedPointer<CNode> dest_node;
    QString src_name;
    QString dest_name;
    QString src_gate;
    QString dest_gate;
    QVariant v;

    v = connections_json["src_node"];
    if(v.isValid()) {
        src_name = v.toString();
    }

    v = connections_json["dest_node"];
    if(v.isValid()) {
        dest_name = v.toString();
    }

    v = connections_json["src_gate"];
    if(v.isValid()) {
        src_gate = v.toString();
    }

    v = connections_json["dest_gate"];
    if(v.isValid()) {
        dest_gate = v.toString();
    }

    // Check that all parameters are there in the json string.
    if(src_name.isEmpty() || src_gate.isEmpty() ||
        dest_name.isEmpty() || dest_gate.isEmpty()) {
        qWarning() << "CNodeMesh::addConnection():"
                   << "Some connection parameters are missing. Connection not created.";
        return false;
    }

    // Get the nodes.
    if(m_nodes.contains(src_name)) {
        src_node = QSharedPointer<CNode>(m_nodes[src_name]);
    }
    if(m_nodes.contains(dest_name)) {
        dest_node = QSharedPointer<CNode>(m_nodes[dest_name]);
    }

    // Make sure the referenced nodes exist.
    if(src_node.isNull()) {
        qWarning() << "CNodeMesh::addConnection():"
                   << "Connection not established. Source node"
                   << src_name << "was not found.";
    }
    if(dest_node.isNull()) {
        qWarning() << "CNodeMesh::addConnection():"
                   << "Connection not established. Destination node"
                   << dest_name << "was not found.";
    }
    if(src_node.isNull() || dest_node.isNull()) {
        return false;
    }

    // Attempt to establish the connection.
    if(!src_node->connect(src_gate, *dest_node, dest_gate)) {
        qWarning() << "CNodeMesh::addConnection():"
                   << "Failed to establish a connection.";
        return false;
    }

    return true;
}

void CNodeMesh::onNodeStarted(bool success)
{
    // Decrease the nodes that have been started and check if there
    // ... are no more pending nodes waiting to finish starting.
    m_nodes_waiting--;

    if(!success) {
        m_start_success = false;
    }

    if(m_nodes_waiting == 0) {
        emit nodesStarted(m_start_success);
    }
}

void CNodeMesh::onNodeProcessing(bool not_idle)
{
    if(not_idle) {
        // A node started processing something.
        m_nodes_processing++;
    }
    else {
        m_nodes_processing--;
    }

    if(m_nodes_processing == 0) {
        // There are no more nodes processing.
        emit simulationFinished();
    }
}
