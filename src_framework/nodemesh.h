#ifndef NODEMESH_H
#define NODEMESH_H

#include "node.h"
#include <QList>
#include <QSharedPointer>

class CNodeMesh
{
  private:
    // All the nodes in this collection.
    QMap<QString, QSharedPointer<CNode>> m_nodes;

  public:
    explicit CNodeMesh();
    // Receive a JSON string to be parsed into Nodes and connections.
    bool parseMesh(QString json_str);
    // Start processing the mesh.
    void start();

  private:
    bool addNode(QVariantMap &node_json);
    bool addConnection(QVariantMap &connections_json);
    // Call the "start" function of all nodes in this mesh.
    void startNodes();
};

#endif // NODEMESH_H
