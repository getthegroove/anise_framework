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

//------------------------------------------------------------------------------
// Constructor and Destructor

CFramework::CFramework(QObject *parent/*= 0*/)
    : QObject(parent)
    , m_mesh()
{

}


//------------------------------------------------------------------------------
// Public Functions

void CFramework::main()
{
    qDebug() << "CFramework.main() Info:: Starting the framework.";

    // Load dynamic nodes and messages into their corresponsing factories.
    // ... The data nodes should be loaded first as the nodes use them.
    CDataFactory::instance().loadLibraries();
    CNodeFactory::instance().loadLibraries();

     QFile file("/home/boy/Documents/CASED/Repos/anids-framework/meshes/basic.mesh");
    // QFile file("/home/boy/Documents/CASED/Repos/anids-framework/meshes/tcpdump.mesh");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    m_mesh.parseMesh(stream.readAll());
    m_mesh.start();

    // CNodeConfig conf;
    // if(!CNodeFactory::instance().configTemplate("file", conf)) {
    //     qDebug() << "CFramework::main() Error:" << "File Node not found." << endl;
    //     QCoreApplication::exit(1);
    // }
    // conf.setParameter("file", "/home/boy/test");

    // conf.setName("Node1");
    // CNode *node1 = CNodeFactory::instance().createNode("file", conf);

    // conf.setName("Node2");
    // CNode *node2 = CNodeFactory::instance().createNode("file", conf);

    // if(!node1->connect("out", *node2, "in")) {
    //     qDebug() << "CFramework::main() Error:"
    //              << "Could not establish node connection." << endl;
    // }

    // node1->start();
}


//------------------------------------------------------------------------------
// Private Functions
