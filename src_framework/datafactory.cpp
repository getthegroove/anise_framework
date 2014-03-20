#include "datafactory.h"
#include <dlfcn.h>
#include <QDebug>


CDataFactory *CDataFactory::m_instance = nullptr;


//------------------------------------------------------------------------------
// Constructor and Destructor

CDataFactory::CDataFactory()
{

}

//------------------------------------------------------------------------------
// Public Functions

CDataFactory &CDataFactory::instance()
{
    if(m_instance == nullptr) {
        m_instance = new CDataFactory();
    }

    return *m_instance;
}

void CDataFactory::loadLibraries(QString folder)
{
    CDynamicFactory::loadLibraries(folder, RTLD_NOW | RTLD_GLOBAL);
}


CData *CDataFactory::createData(QString data_name) const
{
    if(!m_makers.contains(data_name)) {
        return nullptr;
    }

    data_maker_fnc make = m_makers.value(data_name);
    CData *data = make();

    return data;
}

//------------------------------------------------------------------------------
// Protected Functions

void CDataFactory::addLibrary(void *library_handle, QString filename)
{
    // Obtain the name of the message.
    data_name_fnc name = (data_name_fnc)dlsym(library_handle, "name");
    const char *name_chars = name();
    qDebug() << "CDataFactory::addLibrary() Info:"
             << "Loaded Data Structure:" << name_chars << endl;

    // Make sure a node with a similar name has not already been loaded.
    if(m_makers.contains(name_chars)) {
        qDebug() << "CDataFactory::addLibrary() Warning:"
                 << "The Data Factory already loaded a structure called '"
                 << name_chars
                 << "'. Loaded by" << filename << endl;
        return;
    }

    // Register the maker of the message.
    m_makers[name_chars] = (data_maker_fnc)dlsym(library_handle, "maker");
}

