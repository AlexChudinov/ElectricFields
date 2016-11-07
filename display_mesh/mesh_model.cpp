#include "mesh_model.h"

mesh_model::mesh_model(const QSharedPointer<mesh_geom> &mesh, QObject* parent)
    : QStandardItemModel(parent),
      mesh_(mesh)
{}

mesh_model::~mesh_model(){}

void mesh_model::reset_mesh_model()
{
    //Clear all the items
    this->clear();

    //Get boundary region names from the mesh
    QStringList boundary_names_;
    QList<size_t> boundary_sizes_;
    QList<mesh_geom::NODE_TYPE> node_types_;
    mesh_->iterate_over_boundaries
            ([&](const mesh_geom::boundary_entry& b)
    {
        boundary_names_.push_back(b.first.c_str());
        boundary_sizes_.push_back(b.second.size());
        node_types_.push_back(mesh_->get_boundary_type(b.first));
    });

    //Set boundary names in UI
    this->setColumnCount(4);
    this->setRowCount(boundary_names_.size());
    this->setHorizontalHeaderItem(0,new QStandardItem("Name"));
    this->setHorizontalHeaderItem(1,new QStandardItem("Type"));
    this->setHorizontalHeaderItem(2,new QStandardItem("Size"));
    for(int i = 0; i < boundary_names_.size(); ++i)
    {
        this->setItem(i,0,new QStandardItem(boundary_names_[i]));
        this->setItem(i,1,new QStandardItem(QString("%1").arg(i)));
        this->setItem(i,2,new QStandardItem(QString("%1").arg(i)));
    }
}
