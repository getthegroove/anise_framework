#include "framework.h"
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
    qDebug() << "CFramework::main(): Starting the framework.";

    // Load dynamic nodes and messages into their corresponsing factories.
    // ... The data nodes should be loaded first as the nodes use them.
    CDataFactory::instance().loadLibraries();
    CNodeFactory::instance().loadLibraries();

    // Create the mesh.
    initMesh();
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


//------------------------------------------------------------------------------
// Private Functions

void CFramework::simulateMesh()
{
    m_mesh.startSimulation();
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
