#ifndef CFRAMEWORK_H
#define CFRAMEWORK_H

#include "node/nodemesh.h"
#include <QObject>

class CFramework : public QObject
{
  Q_OBJECT

  public:
    CNodeMesh m_mesh;

  public:
    explicit CFramework(QObject *parent = 0);

  public slots:
    // The framework's main funcion.
    void main();

  private:
    // Initialize the nodes' mesh.
    void initMesh();
    // Start the simulation. All the nodes without connected
    // ... inputs are sent a "start" signal.
    void simulateMesh();

    // Functions called through the command line parameters.
    // List all the Nodes that have been loaded.
    void printNodes(bool pretty_print);

  private slots:
    // The mesh has finished initializing all nodes.
    void onMeshInit(bool success);
    void onMeshFinish();
};

#endif // CFRAMEWORK_H
