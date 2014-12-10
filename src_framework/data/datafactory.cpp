#include "datafactory.h"
#include "data.h"
#include "errordata.h"
#include "messagedata.h"
#include <dlfcn.h>
#include <QDebug>
#include <QRegExp>


CDataFactory *CDataFactory::m_instance = nullptr;


//------------------------------------------------------------------------------
// Constructor and Destructor

CDataFactory::CDataFactory()
{
    // Add builtin data classes to the factory.
    registerBuiltinData();
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


CData *CDataFactory::createData(QString data_type_name) const
{
    if(!m_makers.contains(data_type_name)) {
        return nullptr;
    }

    data_maker_fnc make = m_makers.value(data_type_name);
    CData *data = make();
    data->setTypeName(data_type_name);

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
    else {
        qWarning() << "CDataFactory::addLibrary(): "
                   << "Could not load the data file " << filename;
        return;
    }

    qDebug() << "CDataFactory::addLibrary():"
             << "Loaded Data Structure:" << name;

    // Make sure a node with a similar name has not already been loaded.
    if(m_makers.contains(name)) {
        qWarning() << "CDataFactory::addLibrary():"
                   << "The Data Factory already loaded a structure called '"
                   << name
                   << "'. Loaded by" << filename;
        return;
    }

    // Register the maker of the message.
    m_makers[name] = (data_maker_fnc)dlsym(library_handle, "maker");
}


//------------------------------------------------------------------------------
// Private Functions

void  CDataFactory::registerBuiltinData()
{
    // Error data class
    m_makers["error"] = &CErrorData::maker;
    m_makers["message"] = &CMessageData::maker;
}
