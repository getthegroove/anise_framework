#ifndef NODEFACTORY_H
#define NODEFACTORY_H

#include "dynamicfactory.h"
#include <QString>
#include <QMap>

class QObject;
class CNodeConfig;
class CNode;

typedef const char *(*node_name_fnc)();
typedef void *(*node_configure_fnc)(CNodeConfig &);
typedef CNode *(*node_maker_fnc)(const CNodeConfig &config);

class CNodeFactory: public CDynamicFactory
{
  private:
    // Singleton member variable.
    static CNodeFactory *m_instance;
    // Map of functions found in the loaded dynamic libraries.
    QMap<QString, node_maker_fnc> m_makers;
    QMap<QString, node_configure_fnc> m_config_makers;

  public:
    static CNodeFactory &instance();

    // Obtain the configuration template of the supplied node name.
    // ... Return true if the config was created, false if the node
    // ... was not found.
    bool configTemplate(QString node_name, CNodeConfig &config);
    // Create an instance of the supplied node name. Return null
    // ... if the node does not exist. The caller is responsible for
    // ... freeing up the memory after using it.
    CNode *createNode(QString node_name, const CNodeConfig &config);

  private:
    // Singleton class.
    explicit CNodeFactory();
    // Load the dynamic nodes coming from external libraries.
    void addLibrary(void *library_handle, QString filename);
};

#endif // NODEFACTORY_H
