#include "tcpstreamextractornode.h"
#include "data/datafactory.h"
#include "data/messagedata.h"
#include <QDebug>


//------------------------------------------------------------------------------
// Constructor and Destructor

CTcpStreamExtractorNode::CTcpStreamExtractorNode(const CNodeConfig &config,
                                                 QObject *parent/* = 0*/)
    : CNode(config, parent)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CTcpStreamExtractorNode::configure(CNodeConfig &config)
{
    // Add parameters
    //config.addFilename("file", "Input File", "File to be read from disk.");

    // Add the gates.
    config.addInput("in", "tcpdump");
    config.addOutput("out", "tcpstreams");
}

void CTcpStreamExtractorNode::data(QString gate_name,
                                   const CConstDataPointer &data)
{
    // No need to track gates.
    Q_UNUSED(gate_name);

    qDebug() << "Data received.";

    if(data->getType() == "message") {
        auto pmsg = data.staticCast<const CMessageData>();
        QString msg = pmsg->getMessage();
        qDebug() << "Received message:" << msg;
        if(msg == "error") {
            commitError("out", "Could not get tcp file data.");
            return;
        }
    }
}


//------------------------------------------------------------------------------
// Protected Functions


bool CTcpStreamExtractorNode::start()
{
    qDebug() << "Start called.";

    // Commit data to a gate.
    //commit("out", m_table);

    return true;
}

