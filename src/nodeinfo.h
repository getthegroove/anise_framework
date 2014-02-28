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
    static CNodeConfig m_node_config;

  public:
    static void prepareConfigTemplate()
    {
        // Here the node is able to specify configuration options such
        // as the algorithm parameters, file inputs, node visual icon, etc.
        T::configure(m_node_config);
    }

    static const CNodeConfig& getConfigTemplate()
    {
        return m_node_config;
    }
};


// Static member initializations.
template<class T> CNodeConfig CNodeInfo<T>::m_node_config;

#endif // NODEINFO_H
