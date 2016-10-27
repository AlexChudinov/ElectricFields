#ifndef MESH_MODEL_H
#define MESH_MODEL_H

#include <QStandardItemModel>
#include "mesh_geometry.h"

/**
 * Mesh parameters that could be accessed via the QModelView
 */
class mesh_model : public QStandardItemModel
{
    Q_OBJECT

    using mesh_geom = mesh_geometry<float, size_t>;

    const mesh_geom& mesh_;
public:
    mesh_model(const mesh_geom& mesh, QObject *parent = 0);
    ~mesh_model();

public slots:
    void reset_mesh_model();
};

#endif // MESH_MODEL_H
