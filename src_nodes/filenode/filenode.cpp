#include "filenode.h"
#include "data/datafactory.h"
#include "tabledata/tabledata.h"
#include "data/messagedata.h"
#include <QDebug>

//------------------------------------------------------------------------------
// Constructor and Destructor

CFileNode::CFileNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
    , m_data_file()
{

}


//------------------------------------------------------------------------------
// Public Functions

void CFileNode::configure(CNodeConfig &config)
{
    // Add parameters
    config.addFilename("file", "Input File",
                       "Path of the file to read from disk.");
    config.addBool("binary", "Binary format",
                   "Parse the file contents as binary data.");

    // Add inputs and outputs
    config.addOutput("out", "file");
}

//------------------------------------------------------------------------------
// Protected Functions

bool CFileNode::start()
{
    QVariant filename = getConfig().getParameter("file")->value;
    QVariant binary = getConfig().getParameter("binary")->value;

    m_data_file = QSharedPointer<CFileData>(
        static_cast<CFileData *>(createData("file")));

    if(m_data_file->readFile(filename.toString(), binary.toBool())) {
        return true;
    }
    else {
        qWarning() << "CFileNode::start(): The file"
                 << filename.toString()
                 << "could not be opened.";
        return false;
    }
}

void CFileNode::data(QString gate_name, const CConstDataPointer &data)
{
    // No input gates.
    Q_UNUSED(gate_name);

    if(data->getType() == "message") {
        auto pmsg = data.staticCast<const CMessageData>();
        QString msg = pmsg->getMessage();
        if(msg == "start") {
            commit("out", m_data_file);
        }
        else if(msg == "error") {
            qCritical() << "CFileNode::data(): Error found while processing"
                        << "data().";
        }
    }
}
