#include "filenode.h"
#include <QDebug>

const char *name()
{
    return CFileNode::name();
}

void configure(CNodeConfig &config)
{
    CFileNode::configure(config);
}

CNode *maker(const CNodeConfig &config)
{
    return new CFileNode(config);
}


//------------------------------------------------------------------------------
// Constructor and Destructor

CFileNode::CFileNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
    , CNodeInfo<CFileNode>(config)
{

}


//------------------------------------------------------------------------------
// Public Functions

const char *CFileNode::name()
{
    return "File";
}

void CFileNode::configure(CNodeConfig &config)
{
    // Add parameters
    config.addFilename("file", "Input File", "File to be read from disk.");

    // Add inputs and outputs
    config.addInput("Start", "start_data");
    config.addOutput("File Table", "table_data");
}


//------------------------------------------------------------------------------
// Public Slots


//------------------------------------------------------------------------------
// Protected Functions

void CFileNode::process()
{
    qDebug() << "Hello from the deep space of the dynamic library loading thing.";
}


//------------------------------------------------------------------------------
// Private Slots
