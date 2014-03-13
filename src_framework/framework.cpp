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
    qDebug("CFramework.main() Info:: Starting the framework.");

    // Load dynamic nodes and messages into their corresponsing factories.
    CNodeFactory::instance().loadLibraries("./nodes");
    CDataFactory::instance().loadLibraries("./data");

    CNodeConfig conf;
    if(!CNodeFactory::instance().configTemplate("File", conf)) {
        qDebug() << "CFramework::main() Error:" << "File Node not found.";
        QCoreApplication::exit(1);
    }
    conf.setParameter("file", "/home/boy/test");
    CNode *node = CNodeFactory::instance().createNode("File", conf);

    qDebug("CFramework.main() Info:: Exiting the framework.");
    QCoreApplication::exit();
}


//------------------------------------------------------------------------------
// Private Functions
