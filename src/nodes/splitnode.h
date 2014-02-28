#ifndef SPLITNODE_H
#define SPLITNODE_H

#include "../node.h"
#include "../nodeinfo.h"
#include <QObject>

class CSplitNode : public CNode, public CNodeInfo<CSplitNode>
{
  Q_OBJECT

  public:
    explicit CSplitNode(const CNodeConfig &config, QObject *parent = 0);
    static void configure(CNodeConfig &config);

  protected:
    virtual void process();
};

#endif // SPLITNODE_H
