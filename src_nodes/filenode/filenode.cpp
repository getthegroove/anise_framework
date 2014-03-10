#include "filenode.h"
#include <QDebug>

const char *name()
{
    return "File";
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
{

}


//------------------------------------------------------------------------------
// Public Functions

void CFileNode::configure(CNodeConfig &config)
{
    // Add parameters
    config.addFilename("file", "Input File", "File to be read from disk.");

    // Add the gates.
    int inputbox = config.addInputBox();
    config.addInputGate(inputbox, "Start", "start_msg");

    int outputbox = config.addOutputBox();
    config.addOutputGate(outputbox, "File Table", "table_msg");
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
