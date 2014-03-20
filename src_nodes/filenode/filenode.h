#ifndef FILENODE_H
#define FILENODE_H

#include "node.h"
#include "nodeinfo.h"
#include "nodeconfig.h"
#include <QObject>
#include <QString>

class CTableData;

class CFileNode : public CNode, public CNodeInfo<CFileNode>
{
  Q_OBJECT

  private:
    CTableData *m_table;

  public:
    // Constructor
    explicit CFileNode(const CNodeConfig &config, QObject *parent = 0);
    // Static Node Functions
    static const char *name();
    static void configure(CNodeConfig &config);

  public:
    //virtual void data(CData &data);
    virtual void process();

  protected:
    virtual void init(const CDataFactory &data_factory);
};

#endif // FILENODE_H
