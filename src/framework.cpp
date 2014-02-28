#include "framework.h"
#include "nodes/filenode.h"
#include "nodes/splitnode.h"
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
    qDebug("CFramework.main():: Starting the framework.");

    CFileNode::prepareConfigTemplate();
    const CNodeConfig &config = CFileNode::getConfigTemplate();
    config.setParameter("file", "/home/boy/test");

    qDebug("CFramework.main():: Exiting the framework.");
    QCoreApplication::exit();
}


//------------------------------------------------------------------------------
// Private Functions
