#include "framework.h"
#include "nodefactory.h"
#include "datafactory.h"
#include "nodeconfig.h"
#include "node.h"
#include <QDebug>
#include <QCoreApplication>

//------------------------------------------------------------------------------
// Constructor and Destructor

CFramework::CFramework(QObject *parent/*= 0*/): QObject(parent)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CFramework::main()
{
    qDebug() << "CFramework.main() Info:: Starting the framework." << endl;

    // Load dynamic nodes and messages into their corresponsing factories.
    // ... The data nodes should be loaded first as the nodes might use them.
    CDataFactory::instance().loadLibraries();
    CNodeFactory::instance().loadLibraries();

    CNodeConfig conf;
    if(!CNodeFactory::instance().configTemplate("file", conf)) {
        qDebug() << "CFramework::main() Error:" << "File Node not found." << endl;
        QCoreApplication::exit(1);
    }
    conf.setParameter("file", "/home/boy/test");

    conf.setName("Node1");
    CNode *node1 = CNodeFactory::instance().createNode("file", conf);

    conf.setName("Node2");
    CNode *node2 = CNodeFactory::instance().createNode("file", conf);

    if(!node1->connect("out", *node2, "in")) {
        qDebug() << "CFramework::main() Error:"
                 << "Could not establish node connection." << endl;
    }


    if(node1 != nullptr) delete node1;
    if(node2 != nullptr) delete node2;

    qDebug() << "CFramework.main() Info:: Exiting the framework." << endl;
    QCoreApplication::exit();
}


//------------------------------------------------------------------------------
// Private Functions
