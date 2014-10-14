#include "tcpdumpnode.h"
#include "data/datafactory.h"
#include "data/messagedata.h"
#include "filedata/filedata.h"
#include <QDebug>
#include <QFile>

//------------------------------------------------------------------------------
// Constructor and Destructor

CTcpDumpNode::CTcpDumpNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
    , m_tcpdump(nullptr)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CTcpDumpNode::configure(CNodeConfig &config)
{
    // Add parameters
    //config.addFilename("file", "TCP Dump file", "TCP dump file to be read from disk.");

    // Add inputs and outputs
    config.addInput("in", "file");
    config.addOutput("out", "tcpdump");
}


//------------------------------------------------------------------------------
// Protected Functions

void CTcpDumpNode::init(const CDataFactory &data_factory)
{
    qDebug() << "CTcpDumpNode::init(): called.";

    // Create the TCP Dump data structure.
    CTcpDumpData *tcpdump =
            static_cast<CTcpDumpData *>(data_factory.createData("tcpdump"));
    m_tcpdump = QSharedPointer<CTcpDumpData>(tcpdump);
}

bool CTcpDumpNode::start()
{
    return true;
}

void CTcpDumpNode::data(QString gate_name, QSharedPointer<CData> data)
{
    // No need to track gates.
    Q_UNUSED(gate_name);

    QSharedPointer<CFileData> file;

    if(data->getType() == "message") {
        auto pmsg = data.staticCast<CMessageData>();
        QString msg = pmsg->getMessage();
        qDebug() << "CTcpDumpNode::data(): Received message:" << msg;
        if(msg == "error") {
            commitError("out", "Could not get tcp file data.");
            return;
        }
    }
    else if(data->getType() == "file") {
        file = data.staticCast<CFileData>();
        m_tcpdump->parse(file->getBytes());

        commit("out", m_tcpdump);
    }
}
