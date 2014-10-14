#ifndef NODEMESH_H
#define NODEMESH_H

#include "node.h"
#include <QList>
#include <QSharedPointer>
#include <QObject>

class CNodeMesh: public QObject
{
  Q_OBJECT

  private:
    // All the nodes in this collection.
    QMap<QString, QSharedPointer<CNode>> m_nodes;
    qint32 m_nodes_waiting;
    bool m_start_success;
    // The number of nodes that are concurrently processing something
    // ... at any point in the lifetime of the mesh.
    quint32 m_nodes_processing;

  public:
    explicit CNodeMesh();
    // Receive a JSON string to be parsed into Nodes and connections.
    bool parseMesh(QString json_str);
    // Start all the nodes by calling their start() function in parallel.
    void startNodes();
    // Start the mesh by sending a "start" message to all nodes with
    // ... inputs with no connections.
    void startSimulation();

  signals:
    void nodesStarted(bool success);
    void simulationFinished();

  private:
    bool addNode(QVariantMap &node_json);
    bool addConnection(QVariantMap &connections_json);

  private slots:
    void onNodeStarted(bool success);
    void onNodeProcessing(bool not_idle);

};

#endif // NODEMESH_H
