#include "framework.h"
#include "messagehandler.h"
#include "node/nodefactory.h"
#include "data/datafactory.h"
#include "node/nodeconfig.h"
#include "node/node.h"
#include "node/nodemesh.h"
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QThreadPool>
#include <QObject>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

//------------------------------------------------------------------------------
// Constructor and Destructor

CFramework::CFramework(QObject *parent/*= 0*/)
    : QObject(parent)
    , m_mesh()
{
    // Create the mesh connections.
    QObject::connect(&m_mesh, SIGNAL(nodesStarted(bool)),
                     this, SLOT(onMeshInit(bool)));
    QObject::connect(&m_mesh, SIGNAL(simulationFinished()),
                     this, SLOT(onMeshFinish()));
}


//------------------------------------------------------------------------------
// Public Functions

void CFramework::main()
{
    // Parse command line parameters.
    QCommandLineParser parser;
    parser.setApplicationDescription(
        "The ANISE Framework is an environment for the development and execution "
        "of machine learning algorithms. It focuses on speed and efficiency.");
    parser.addHelpOption();
    parser.addVersionOption();

    // Set the command line options.
    // The --machine option: Output is meant to be processed by a program
    // ... rather than a human.
    QCommandLineOption machineOption("machine",
        "The output is printed in a more friendly way for the purpose of being parsed.");
    parser.addOption(machineOption);
    // The --nodes option
    QCommandLineOption nodesOption("nodes", "Print all the nodes the framework recognizes.");
    parser.addOption(nodesOption);

    parser.process(*QCoreApplication::instance());


    // Are we printing to the console for the humans or for the machines?
    if(!parser.isSet(machineOption)) {
        // Enable pretty printing with out own custom message writer.
        // QT4:
     // qInstallMsgHandler(customMessageWriterQt4);
        // QT5:
        qInstallMessageHandler(humanMessageWriterQt5);
    }
    else {
        qInstallMessageHandler(machineMessageWriterQt5);
    }


    // Load dynamic nodes and messages into their corresponsing factories.
    // ... The data nodes should be loaded first as the nodes use them.
    CDataFactory::instance().loadLibraries();
    CNodeFactory::instance().loadLibraries();

    // Evaluate the parameters
    if(parser.isSet(nodesOption)) {
        printNodes(!parser.isSet(machineOption));
        QCoreApplication::exit(0);
        return;
    }


    // Create the mesh.
    initMesh();
}


//------------------------------------------------------------------------------
// Private Functions

void CFramework::simulateMesh()
{
    m_mesh.startSimulation();
}

void CFramework::printNodes(bool pretty_print)
{
    QJsonArray json_nodes;
    QStringList node_names = CNodeFactory::instance().availableNodes();
    CNodeConfig config;

    for(int i = 0; i < node_names.size(); ++i) {
        QJsonObject json_node;
        CNodeFactory::instance().configTemplate(node_names.at(i), config);
        json_node["name"] = node_names.at(i);

        QJsonArray json_input_gates;
        QJsonArray json_output_gates;
        // Get the input gates as JSON objects.
        const QList<CNodeConfig::SGateTemplate> input_gates = \
                config.getInputTemplates();
        for(int i = 0; i < input_gates.size(); ++i) {
            QJsonObject json_gate;
            json_gate["name"] = input_gates.at(i).name;
            json_gate["type"] = input_gates.at(i).msg_type;
            json_input_gates.append(json_gate);
        }
        // Get the output gates as JSON objects.
        const QList<CNodeConfig::SGateTemplate> output_gates = \
                config.getOutputTemplates();
        for(int i = 0; i < output_gates.size(); ++i) {
            QJsonObject json_gate;
            json_gate["name"] = output_gates.at(i).name;
            json_gate["type"] = output_gates.at(i).msg_type;
            json_output_gates.append(json_gate);
        }

        json_node["input_gates"] = json_input_gates;
        json_node["output_gates"] = json_output_gates;

        json_nodes.append(json_node);
    }

    // Print the JSON representation of all nodes to the console.
    QJsonObject json_container;
    json_container["nodes"] = json_nodes;
    QJsonDocument json_doc(json_container);

    if(pretty_print) {
        qDebug() << "Available Nodes: "
                 << endl << json_doc.toJson(QJsonDocument::Indented);
    }
    else {
        // Info messages disabled normally. Force printing by stating with '@'.
        qDebug() << "@" << json_doc.toJson(QJsonDocument::Compact);
    }
}

void CFramework::onMeshInit(bool success)
{
    if(!success) {
        qCritical() << "CFramework::onMeshInit(): Simulation not started."
                    << "Not all nodes started correctly.";
        QCoreApplication::exit(1);
    }
    else {
        // Start the simulation.
        simulateMesh();
    }
}

void CFramework::onMeshFinish()
{
    qDebug() << "CFramework::onMeshFinished(): Simulation finished.";
    QCoreApplication::exit(0);
}

void CFramework::initMesh()
{
    QFile file("/home/boy/Documents/CASED/Repos/anids-framework/meshes/basic.mesh");
    // QFile file("/home/boy/Documents/CASED/Repos/anids-framework/meshes/tcpdump.mesh");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    m_mesh.parseMesh(stream.readAll());

    // Start the nodes in the mesh.
    m_mesh.startNodes();
}
