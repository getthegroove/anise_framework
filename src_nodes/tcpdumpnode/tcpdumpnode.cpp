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
    // Add inputs and outputs
    config.addInput("in", "file");
    config.addOutput("out", "tcpdump");
}


//------------------------------------------------------------------------------
// Protected Functions

//void CTcpDumpNode::init(const CDataFactory &data_factory)
//{
//    qDebug() << "CTcpDumpNode::init(): called.";
//
//    // Create the TCP Dump data structure.
//    CTcpDumpData *tcpdump =
//            static_cast<CTcpDumpData *>(data_factory.createData("tcpdump"));
//    m_tcpdump = QSharedPointer<CTcpDumpData>(tcpdump);
//}

bool CTcpDumpNode::start()
{
    m_tcpdump = QSharedPointer<CTcpDumpData>(
        static_cast<CTcpDumpData *>(createData("tcpdump")));

    if(!m_tcpdump.isNull()) {
        return true;
    }
    else {
        return false;
    }
}

void CTcpDumpNode::data(QString gate_name, const CConstDataPointer &data)
{
    // No need to track gates.
    Q_UNUSED(gate_name);

    if(data->getType() == "message") {
        auto pmsg = data.staticCast<const CMessageData>();
        QString msg = pmsg->getMessage();
        qDebug() << "CTcpDumpNode::data(): Received message:" << msg;
        if(msg == "error") {
            commitError("out", "Could not get tcp file data.");
            return;
        }
    }
    else if(data->getType() == "file") {
        QSharedPointer<const CFileData> file = data.staticCast<const CFileData>();
        m_tcpdump->parse(file->getBytes());
        qDebug() << "CTcpDumpNode::data(): Packets parsed:"
                 << m_tcpdump->availablePackets();

        commit("out", m_tcpdump);
    }
}
