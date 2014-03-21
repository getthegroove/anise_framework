#ifndef FILENODE_H
#define FILENODE_H

#include "node.h"
#include "nodeconfig.h"
#include <QObject>
#include <QString>

class CTableData;

class CFileNode : public CNode
{
  Q_OBJECT

  private:
    CTableData *m_table;

  public:
    // Constructor
    explicit CFileNode(const CNodeConfig &config, QObject *parent = 0);
    // Static Node Functions
    static void configure(CNodeConfig &config);

  public:
    //virtual void data(CData &data);
    void process();

  public slots:
    void data();

  protected:
    virtual void init(const CDataFactory &data_factory);
};

#endif // FILENODE_H
