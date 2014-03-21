#include "datafactory.h"
#include <dlfcn.h>
#include <QDebug>
#include <QRegExp>


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

void CDataFactory::loadLibraries()
{
    CDynamicFactory::loadLibraries("./data", "lib*data.so",
        RTLD_NOW | RTLD_GLOBAL);
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
    QString name;

    // Obtain the name of the message.
    QRegExp regexp(".*lib(\\w+)data.so$");
    if(regexp.indexIn(filename) != -1) {
        name = regexp.cap(1);
    }

    qDebug() << "CDataFactory::addLibrary() Info:"
             << "Loaded Data Structure:" << name << endl;

    // Make sure a node with a similar name has not already been loaded.
    if(m_makers.contains(name)) {
        qDebug() << "CDataFactory::addLibrary() Warning:"
                 << "The Data Factory already loaded a structure called '"
                 << name
                 << "'. Loaded by" << filename << endl;
        return;
    }

    // Register the maker of the message.
    m_makers[name] = (data_maker_fnc)dlsym(library_handle, "maker");
}

