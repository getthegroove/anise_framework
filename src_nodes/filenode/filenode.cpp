#include "filenode.h"
#include "datafactory.h"
#include "tabledata/tabledata.h"
#include <QDebug>

//------------------------------------------------------------------------------
// Constructor and Destructor

CFileNode::CFileNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
    , m_table(nullptr)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CFileNode::configure(CNodeConfig &config)
{
    // Add parameters
    config.addFilename("file", "Input File", "File to be read from disk.");

    // Add inputs and outputs
    config.addInput("in", "misc");
    config.addOutput("out", "misc");
}

void CFileNode::process()
{
    qDebug() << "Hello from the deep space of the dynamic library loading thing." << endl;

}

void CFileNode::data()
{
    QList<int> list;
    list << 1 << 2 << 3;
    m_table->addRow(list);
    qDebug() << "Data processing done" << endl;
}

//------------------------------------------------------------------------------
// Protected Functions

void CFileNode::init(const CDataFactory &data_factory)
{
    qDebug() << "CFileNode.init() Info: Init Called." << endl;
    m_table = static_cast<CTableData *>(data_factory.createData("table"));
}


