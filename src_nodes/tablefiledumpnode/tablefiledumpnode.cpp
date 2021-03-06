#include "tablefiledumpnode.h"
#include "data/datafactory.h"
#include "data/messagedata.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>


//------------------------------------------------------------------------------
// Constructor and Destructor

CTableFileDumpNode::CTableFileDumpNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CTableFileDumpNode::configure(CNodeConfig &config)
{
    config.setDescription("Write any received table data to a file.");

    // Add parameters
    config.addFilename("filename", "Output File", "File to be written.");
    config.addBool("append", "Append the table data",
                   "Append the data to the output file instead of replacing it.",
                   false);

    // Add the gates.
    config.addInput("in", "table");
}


//------------------------------------------------------------------------------
// Protected Functions

bool CTableFileDumpNode::start()
{
    return true;
}

void CTableFileDumpNode::data(QString gate_name, const CConstDataPointer &data)
{
    Q_UNUSED(gate_name);

    // Process framework messages.
    if(data->getType() == "message") {
        auto pmsg = data.staticCast<const CMessageData>();
        QString msg = pmsg->getMessage();
        qDebug() << "Received message:" << msg;
        if(msg == "error") {
            commitError("out", "Could not get tcp file data.");
            return;
        }
    }
    else if(data->getType() == "table") {
        auto table = data.staticCast<const CTableData>();

        // Get the node properties for the file.
        QString filename = getConfig().getParameter("filename")->value.toString();
        bool append = getConfig().getParameter("append")->value.toBool();

        // Print the table data into the user-supplied filename.
        if(printTable(table, filename, append)) {
            qDebug() << "Table data written into" << filename;
        }
        else {
            qWarning() << "Table data was NOT able to be written into" << filename;
        }
    }
}

bool CTableFileDumpNode::printTable(QSharedPointer<const CTableData> &table,
                                    QString filename, bool append)
{
    QFile file(filename);
    QIODevice::OpenMode flags = QIODevice::WriteOnly | QIODevice::Text;

    if(append) {
        flags |= QIODevice::Append;
    } else {
        flags |= QIODevice::Truncate;
    }

    if(!file.open(flags)){
        return false;
    }

    QTextStream out(&file);

    // Print table columns
    out << table->headerSize() << endl;

    // Print table header.
    const QList<QString> &header = table->getHeader();
    for(const QString& attr : header) {
        out << attr << ' ';
    }
    out << endl;

    // Print each row into the file.
    qint32 row_count = table->getRowCount();
    qint32 col_count = 0;

    for(qint32 i = 0; i < row_count; ++i) {
        const QList<QVariant> &row = table->getRow(i);
        col_count = row.size();
        for(qint32 j = 0; j < col_count; ++j) {
            out << row.at(j).toString();
            if(j != col_count - 1) {
                out << " ";
            }
        }
        out << endl;
    }

    return true;
}

