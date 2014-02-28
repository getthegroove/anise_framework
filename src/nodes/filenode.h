#ifndef FILENODE_H
#define FILENODE_H

#include "../node.h"
#include "../nodeinfo.h"
#include <QObject>

class CNodeConfig;

class CFileNode : public CNode, public CNodeInfo<CFileNode>
{
  Q_OBJECT

  public:
    explicit CFileNode(const CNodeConfig &config, QObject *parent = 0);
    static void configure(CNodeConfig &config);

  protected:
    virtual void process();
};

#endif // FILENODE_H
