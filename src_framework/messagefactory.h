#ifndef MESSAGEFACTORY_H
#define MESSAGEFACTORY_H

#include "dynamicfactory.h"
#include <QMap>
#include <QString>

class CMessage;

typedef CMessage *(*message_maker_fnc)();
typedef const char *(*message_name_fnc)();

class CMessageFactory: public CDynamicFactory
{
  private:
    // Singleton member variable.
    static CMessageFactory *m_instance;
    QMap<QString, message_maker_fnc> m_makers;

  public:
    static CMessageFactory &instance();

  protected:
    // For every library found, this function is called to add
    // ... the required functions to out maps.
    void addLibrary(void *library_handle, QString filename);

  private:
    // We are a singleton.
    explicit CMessageFactory();

};

#endif // MESSAGEFACTORY_H
