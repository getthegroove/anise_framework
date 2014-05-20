#include "framework.h"
#include "nodefactory.h"
#include "datafactory.h"
#include "nodeconfig.h"
#include "node.h"
#include "nodemesh.h"
#include <QDebug>
#include <QCoreApplication>
#include <QFile>

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
    qDebug() << "CFramework.main() Info:: Starting the framework." << endl;

    // Load dynamic nodes and messages into their corresponsing factories.
    // ... The data nodes should be loaded first as the nodes use them.
    CDataFactory::instance().loadLibraries();
    CNodeFactory::instance().loadLibraries();

    QFile file("/home/boy/Documents/CASED/Repos/anids-framework/meshes/basic.mesh");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    m_mesh.parseMesh(stream.readAll());
    m_mesh.start("Node1");

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
