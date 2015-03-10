#ifndef NODEFACTORY_H
#define NODEFACTORY_H

#include "../dynamicfactory.h"
#include <QString>
#include <QMap>

class QObject;
class CNodeConfig;
class CNode;

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

    void loadLibraries();

    // Return true if the specified node class is available.
    bool nodeAvailable(QString node_class);
    // Obtain the configuration template of the supplied node name.
    // ... Return true if the config was created, false if the node
    // ... was not found.
    bool configTemplate(QString node_class_name, CNodeConfig &config);
    // Create an instance of the supplied node name. Return null
    // ... if the node does not exist. The caller is responsible for
    // ... freeing up the memory after using it.
    CNode *createNode(QString node_class_name, const CNodeConfig &config);
    // Return a list of all the available nodes.
    QStringList availableNodes();

  private:
    // Singleton class.
    explicit CNodeFactory();
    // Load the dynamic nodes coming from external libraries.
    void addLibrary(void *library_handle, QString filename);
};

#endif // NODEFACTORY_H
