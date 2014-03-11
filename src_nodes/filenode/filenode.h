#ifndef FILENODE_H
#define FILENODE_H

#include "node.h"
#include "nodeinfo.h"
#include "nodeconfig.h"
#include <QObject>
#include <QString>

extern "C"
{
    const char *name();
    void configure(CNodeConfig &config);
    CNode *maker(const CNodeConfig &config);
}

class CFileNode : public CNode, public CNodeInfo<CFileNode>
{
  Q_OBJECT

  public:
    // Constructor
    explicit CFileNode(const CNodeConfig &config, QObject *parent = 0);
    // Static Node Functions
    static const char *name();
    static void configure(CNodeConfig &config);

  protected:
    virtual void process();
};

#endif // FILENODE_H
