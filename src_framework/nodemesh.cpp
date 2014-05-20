#include "nodemesh.h"
#include "nodefactory.h"
#include "../src_common/qt-json/json.h"
#include <QDebug>

//------------------------------------------------------------------------------
// Constructor and Destructor

CNodeMesh::CNodeMesh()
{

}


//------------------------------------------------------------------------------
// Public Functions

bool CNodeMesh::parseMesh(QString json_str)
{
    QVariant json_variant = QtJson::parse(json_str).toMap();

    if(json_variant.isNull()) {
        qDebug() << "CNodeMesh.parseMesh():: Error: Failed to parse mesh file."
                 << endl;
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

void CNodeMesh::start()
{
    startNodes();
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
        qDebug() << "CNodeMesh.addNode():: Warning:"
                 << "The JSON Node definition did not include class or name."
                 << endl;
        return false;
    }

    // Verify that a Node with the same name does not exist already in the map.
    if(m_nodes.contains(node_name)) {
        qDebug() << "CNodeMesh.addNode():: Warning:"
                 << "A node with the name '" << node_name
                 << "' has already been defined." << endl;
        return false;
    }

    // Get the configuration template of the requested node.
    ok = CNodeFactory::instance().configTemplate(
        node_class, conf);
    if(!ok) {
        // The template for the desired Node class was not found.
        qDebug() << "CNodeMesh.addNode():: Warning: The node class '"
                 << node_class << "' was not found." << endl;
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
                qDebug() << "CNodeMesh.addNode():: Warning:"
                         << "Failed to set the parameter '" << key.toString()
                         << "' in Node '" << node_name
                         << "'." << endl;
                return false;
            }
        }
    }

    // Create the node we've been asked for.
    CNode *node = CNodeFactory::instance().createNode(node_class, conf);
    if(node == nullptr) {
        qDebug() << "CNodeMesh.addNode():: Warning:"
                 << "Could not create Node '" << node_class
                 << "'" << endl;
        return false;
    }
    m_nodes.insert(node_name, QSharedPointer<CNode>(node));

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
        qDebug() << "CNodeMesh.addConnection():: Warning:"
                 << "Some connection parameters are missing. Connection not created."
                 << endl;
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
        qDebug() << "CNodeMesh.addConnection():: Warning:"
                 << "Connection not established. Source node (" << src_name
                 << ") was not found." << endl;
    }
    if(dest_node.isNull()) {
        qDebug() << "CNodeMesh.addConnection():: Warning:"
                 << "Connection not established. Destination node (" << dest_name
                 << ") was not found." << endl;
    }
    if(src_node.isNull() || dest_node.isNull()) {
        return false;
    }

    // Attempt to establish the connection.
    if(!src_node->connect(src_gate, *dest_node, dest_gate)) {
        qDebug() << "CNodeMesh.addConnection():: Warning:"
                 << "Failed to establish a connection."
                 << endl;
        return false;
    }

    return true;
}

void CNodeMesh::startNodes()
{
    QMap<QString, QSharedPointer<CNode>>::iterator i;
    for(i = m_nodes.begin(); i != m_nodes.end(); ++i) {
        auto node = i.value();
        node->start();
    }
}

