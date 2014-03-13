#ifndef DATAFACTORY_H
#define DATAFACTORY_H

#include "dynamicfactory.h"
#include <QMap>
#include <QString>

class CMessage;

typedef CMessage *(*data_maker_fnc)();
typedef const char *(*data_name_fnc)();

class CDataFactory: public CDynamicFactory
{
  private:
    // Singleton member variable.
    static CDataFactory *m_instance;
    QMap<QString, data_maker_fnc> m_makers;

  public:
    static CDataFactory &instance();

  protected:
    // For every library found, this function is called to add
    // ... the required functions to out maps.
    void addLibrary(void *library_handle, QString filename);

  private:
    // We are a singleton.
    explicit CDataFactory();

};

#endif // MESSAGEFACTORY_H
