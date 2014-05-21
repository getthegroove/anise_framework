#include "skeleton.h"
#include "datafactory.h"
#include <QDebug>


//------------------------------------------------------------------------------
// Constructor and Destructor

CSkeleton::CSkeleton(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CSkeleton::configure(CNodeConfig &config)
{
    // Add parameters
    //config.addFilename("file", "Input File", "File to be read from disk.");

    // Add the gates.
    //config.addInput("in", "misc");
    //config.addOutput("out", "misc");
}

void CSkeleton::data(QSharedPointer<CData> data)
{
    // Empty because data is not received by this node.
    qDebug() << "CSkeleton.data():: Info:"  << getConfig().getName()
             << ": Data received.";
}


//------------------------------------------------------------------------------
// Protected Functions

void CSkeleton::init(const CDataFactory &data_factory)
{
    qDebug() << "CSkeleton.init():: Info:" << getConfig().getName()
             << ": Init called.";
}

void CSkeleton::start()
{
    qDebug() << "CSkeleton.start():: Info:" << getConfig().getName()
             << ": Start called.";

    // if(inputLinkCount("in") != 0) {
    //     // If someone is connected do not perform anything at the start.
    //     return;
    // }

    // Commit data to a gate.
    //commit("out", m_table);
}
