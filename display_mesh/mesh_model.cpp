#include "mesh_model.h"

mesh_model::mesh_model(QObject* parent)
    : QStandardItemModel(parent)
{}

mesh_model::~mesh_model(){}

void mesh_model::reset_mesh_model(const QSharedPointer<mesh_geom> &mesh)
{
    mesh_ = mesh;
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
        boundary_sizes_.push_back(b.second.second.size());
        node_types_.push_back(mesh_->get_boundary_type(b.first));
    });

    //Set boundary names in UI
    this->setColumnCount(4);
    this->setRowCount(boundary_names_.size());
    this->setHorizontalHeaderItem(0,new QStandardItem("Name"));
    this->setHorizontalHeaderItem(1,new QStandardItem("Type"));
    this->setHorizontalHeaderItem(2,new QStandardItem("Size"));
    this->setHorizontalHeaderItem(3, new QStandardItem("Value"));
    for(int i = 0; i < boundary_names_.size(); ++i)
    {
        this->setItem(i, 0, new QStandardItem(boundary_names_[i]));
        this->item(i, 0)->setEditable(false);
        this->setItem(i, 1, new QStandardItem(QString("Fixed value")));
        this->setItem(i, 2, new QStandardItem(QString("%1").arg(boundary_sizes_[i])));
        this->item(i, 2)->setEditable(false);
        this->setItem(i, 3, new QStandardItem(QString("0.0")));
    }
}

mesh_model_view::mesh_model_view(QWidget *parent)
    :QTableView(parent),
      model_(new mesh_model(this))
{
    this->setModel(model_);
}

mesh_model_view::~mesh_model_view(){}

void mesh_model_view::reset_mesh_model(const QSharedPointer<mesh_geom> &mesh)
{
    model_->reset_mesh_model(mesh);
    this->update();
}

