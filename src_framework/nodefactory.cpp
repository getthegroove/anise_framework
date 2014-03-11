#include "nodefactory.h"
#include <dlfcn.h>
#include <QDebug>


CNodeFactory *CNodeFactory::m_instance = nullptr;


//------------------------------------------------------------------------------
// Constructor and Destructor

CNodeFactory::CNodeFactory()
{

}


//------------------------------------------------------------------------------
// Public Functions

CNodeFactory &CNodeFactory::instance()
{
    if(m_instance == nullptr) {
        m_instance = new CNodeFactory();
    }

    return *m_instance;
}

bool CNodeFactory::configTemplate(QString node_name, CNodeConfig &config)
{
    if(!m_config_makers.contains(node_name)) {
        return false;
    }

    node_configure_fnc configure = m_config_makers.value(node_name);
    configure(config);

    return true;
}

CNode *CNodeFactory::createNode(QString node_name, const CNodeConfig &config)
{
    if(!m_makers.contains(node_name)) {
        return nullptr;
    }

    node_maker_fnc make = m_makers.value(node_name);
    CNode *node = make(config);

    return node;
}


//------------------------------------------------------------------------------
// Protected Functions

void CNodeFactory::addLibrary(void *library_handle, QString filename)
{
    // Obtain the name of the node.
    node_name_fnc name = (node_name_fnc)dlsym(library_handle, "name");
    const char *name_chars = name();
    qDebug() << "CNodeFactory::loadNodes() Info:"
             << "Loaded Node:" << name_chars << endl;

    // Make sure a node with a similar name has not already been loaded.
    if(m_makers.contains(name_chars)) {
        qDebug() << "CNodeFactory::loadNodes() Warning:"
                 << "The Node Factory already loaded a node called '"
                 << name_chars
                 << "'. Loaded by" << filename << endl;
        return;
    }

    // Register the maker of the node.
    m_makers[name_chars] = (node_maker_fnc)dlsym(library_handle, "maker");
    // Register the configurator of the node.
    m_config_makers[name_chars] = (node_configure_fnc)dlsym(library_handle, "configure");
}
