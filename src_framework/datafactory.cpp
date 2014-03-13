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


//------------------------------------------------------------------------------
// Protected Functions

void CDataFactory::addLibrary(void *library_handle, QString filename)
{
    // Obtain the name of the message.
    data_name_fnc name = (data_name_fnc)dlsym(library_handle, "name");
    const char *name_chars = name();
    qDebug() << "CMessageFactory::addLibrary() Info:"
             << "Loaded Message:" << name_chars << endl;

    // Make sure a node with a similar name has not already been loaded.
    if(m_makers.contains(name_chars)) {
        qDebug() << "CMessageFactory::addLibrary() Warning:"
                 << "The Message Factory already loaded a message called '"
                 << name_chars
                 << "'. Loaded by" << filename << endl;
        return;
    }

    // Register the maker of the message.
    m_makers[name_chars] = (data_maker_fnc)dlsym(library_handle, "maker");
}
