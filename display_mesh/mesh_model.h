#ifndef MESH_MODEL_H
#define MESH_MODEL_H

#include <QStandardItemModel>
#include <QTableView>
#include "mesh_math/mesh_geometry.h"

/**
 * Mesh parameters that could be accessed via the QModelView
 */
class mesh_model : public QStandardItemModel
{
    Q_OBJECT

    using mesh_geom = mesh_geometry<float, uint32_t>;

    /**
     * Mesh data
     */
    QSharedPointer<mesh_geom> mesh_;

public:
    mesh_model(QObject *parent = 0);
    ~mesh_model();

public slots:
    /**
     * Sets new mesh data pointer
     */
    void reset_mesh_model(const QSharedPointer<mesh_geom>& mesh);
};

/**
 * Mesh model view in a table
 */
class mesh_model_view : public QTableView
{
    Q_OBJECT
    using mesh_geom = mesh_geometry<float, size_t>;



};

#endif // MESH_MODEL_H
