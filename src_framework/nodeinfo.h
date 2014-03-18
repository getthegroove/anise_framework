#ifndef NODEINFO_H
#define NODEINFO_H

#include "nodeconfig.h"

// Helper class that keeps the configuration parameters of a node. It
// ... also forces any derived class to implement some special
// ... static functions for configuring the node.
template<class T>
class CNodeInfo
{
  private:
    const CNodeConfig m_node_config;

  public:
    explicit CNodeInfo(const CNodeConfig &config)
    : m_node_config(config) {}

    static const char *name()
    {
        return T::name();
    }

    static void configure(CNodeConfig &config)
    {
        // Here the node is able to specify configuration options such
        // as the algorithm parameters, file inputs, node visual icon, etc.
        T::configure(config);
    }

    const CNodeConfig& getConfig() const
    {
        return m_node_config;
    }
};

#endif // NODEINFO_H
