#include "application_data.h"
#include "data_export/ansys_mesh_export.h"

#include <QMessageBox>
#include <QFile>
#include <QTextStream>

application_data::application_data(QObject* parent)
    :
      QThread(parent),
      mesh_geometry_(nullptr),
      process_(&application_data::load_mesh)
{}

application_data::~application_data()
{
    this->clear_mesh_();
}

void application_data::run()
{
    (this->*(this->process_))(this->file_name_);
}

void application_data::set_process(application_data::PROCESS_ID process_id)
{
    switch(process_id)
    {
    case LOAD_MESH:
        this->process_ = &application_data::load_mesh;
    default:
        return;
    }
}

void application_data::clear_mesh_()
{
    if(this->mesh_geometry_) delete this->mesh_geometry_;
    this->mesh_geometry_ = nullptr;
}

void application_data::load_mesh(QString fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        this->clear_mesh_();
        size_t progress_val = 0;
        QTextStream stream(&file);
        this->mesh_geometry_ = new mesh_geom(
                    parse_ansys_mesh<float, uint32_t>
                    (
                        stream,
                        [&progress_val, this](QTextStream& stream)
        {
            stream.readLine();
            emit this->progress_value(progress_val);
            progress_val += 10;
        }));
        emit this->mesh_loaded(mesh_geometry_);
    }
    else
    {
        QMessageBox::warning(
                    0,
                    "Mesh reading",
                    "Could not open file!");
    }
}

