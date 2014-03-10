#ifndef NODEFACTORY_H
#define NODEFACTORY_H

#include <QString>
#include <QMap>

class QObject;
class CNodeConfig;
class CNode;

typedef const char *(*name_fnc)();
typedef void *(*configure_fnc)(CNodeConfig &);
typedef CNode *(*maker_fnc)(const CNodeConfig &config);

class CNodeFactory
{
  private:
    static CNodeFactory *m_instance;
    QMap<QString, maker_fnc> m_makers;
    QMap<QString, configure_fnc> m_config_makers;

  public:
    static CNodeFactory &instance();

    // Load all nodes found in the given directory.
    void loadNodes(QString folder);
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
};

#endif // NODEFACTORY_H
