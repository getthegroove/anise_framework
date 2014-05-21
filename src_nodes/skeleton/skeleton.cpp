#include "skeleton.h"
#include "datafactory.h"
#include <QDebug>


//------------------------------------------------------------------------------
// Constructor and Destructor

CFileNode::CFileNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CFileNode::configure(CNodeConfig &config)
{
    // Add parameters
    //config.addFilename("file", "Input File", "File to be read from disk.");

    // Add the gates.
    //config.addInput("in", "misc");
    //config.addOutput("out", "misc");
}

void CFileNode::data(QSharedPointer<CData> data)
{
    // Empty because data is not received by this node.
    qDebug() << "CFileNode.data():: Info:"  << getConfig().getName()
             << ": Data received.";
}


//------------------------------------------------------------------------------
// Protected Functions

void CFileNode::init(const CDataFactory &data_factory)
{
    qDebug() << "CFileNode.init():: Info:" << getConfig().getName()
             << ": Init called.";
}

void CFileNode::start()
{
    qDebug() << "CFileNode.start():: Info:" << getConfig().getName()
             << ": Start called.";

    // if(inputLinkCount("in") != 0) {
    //     // If someone is connected do not perform anything at the start.
    //     return;
    // }

    // Commit data to a gate.
    //commit("out", m_table);
}
