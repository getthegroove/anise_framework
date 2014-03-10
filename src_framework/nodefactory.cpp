#include "nodefactory.h"
#include <dlfcn.h>
#include <QDir>
#include <QStringList>
#include <QDebug>

#include <iostream>

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

void CNodeFactory::loadNodes(QString folder)
{
    // Look into the "./nodes" dictionary.
    QDir dir(folder);
    dir.setFilter(QDir::Files);
    // Only list files ending with .so
    QStringList filter;
    filter << "*.so";
    dir.setNameFilters(filter);

    QStringList list = dir.entryList();

    for(auto it = list.begin(); it != list.end(); ++it) {
        QString filename;
        filename = folder + "/" + (*it).toLocal8Bit().constData();
        // Open the node file.
        void *handle = dlopen(filename.toLocal8Bit().constData(), RTLD_NOW);
        if(handle == NULL) {
            qDebug() << "CNodeFactory::loadNodes() Error:" << endl
                     << "There was an error loading the node '"
                     << (*it) << "'." << dlerror() << endl;
            continue;
        }

        // Obtain the name of the node.
        name_fnc name = (name_fnc)dlsym(handle, "name");
        const char *name_string = name();
        qDebug() << "CNodeFactory::loadNodes() Info:"
                 << "Loaded Node:" << name_string << endl;

        // Make sure a node with a similar name has not already been loaded.
        if(m_makers.contains(name_string)) {
            qDebug() << "CNodeFactory::loadNodes() Warning:"
                     << "The Node Factory already loaded a node called '"
                     << name
                     << "'. Loaded by" << filename << endl;
            continue;
        }

        // Register the maker of the node.
        m_makers[name_string] = (maker_fnc)dlsym(handle, "maker");
        // Register the configurator of the node.
        m_config_makers[name_string] = (configure_fnc)dlsym(handle, "configure");
    }
}

bool CNodeFactory::configTemplate(QString node_name, CNodeConfig &config)
{
    if(!m_config_makers.contains(node_name)) {
        return false;
    }

    configure_fnc configure = m_config_makers.value(node_name);
    configure(config);
    return true;
}

CNode *CNodeFactory::createNode(QString node_name, const CNodeConfig &config)
{
    if(!m_makers.contains(node_name)) {
        return nullptr;
    }

    maker_fnc make = m_makers.value(node_name);
    CNode *node = make(config);

    return node;
}


//------------------------------------------------------------------------------
// Public Slots


//------------------------------------------------------------------------------
// Private Functions


//------------------------------------------------------------------------------
// Private Slots

