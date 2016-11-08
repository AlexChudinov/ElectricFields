#include "application_data.h"
#include "data_export/ansys_mesh_export.h"

#include <QFile>
#include <QTextStream>

Q_DECLARE_METATYPE(QSharedPointer<mesh_geom>)

application_data::application_data(QObject* parent)
    :
      QThread(parent),
      process_(&application_data::load_mesh)
{
    qRegisterMetaType<QSharedPointer<mesh_geom>>("QSharedPointer<mesh_geom>");
    connect(this, SIGNAL(started()), this, SIGNAL(application_data_holder_busy()));
    connect(this, SIGNAL(finished()), this, SIGNAL(application_data_holder_free()));
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
        break;
    case LOAD_BOUNDARY:
        this->process_ = &application_data::load_boundary_regions;
        break;
    default:
        return;
    }
}

void application_data::load_mesh(QString file_name)
{
    QFile file(file_name);
    if(file.open(QIODevice::ReadOnly))
    {
        size_t progress_val = 0;
        QTextStream stream(&file);
        this->mesh_geometry_ = QSharedPointer<mesh_geom>::create(
                    parse_ansys_mesh(stream,
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
        emit this->warning("Could not open file!");
    }
}

void application_data::load_boundary_regions(QString file_name)
{
    QFile file(file_name);
    if(file.open(QIODevice::ReadOnly))
    {
        size_t progress_val = 0;
        QTextStream stream(&file);

        float progress_coeff;
        stream >> progress_coeff;
        stream.seek(0); //return back
        progress_coeff = 100. / progress_coeff;

        if(!parse_ansys_boundary_mesh(*this->mesh_geometry_, stream,
                                      [&progress_val, &progress_coeff, this]()
        {
            emit this->progress_value(progress_val);
            progress_val += progress_coeff;
        }))
            emit this->warning("Some errors occur during the boundary mesh reading!");

        emit this->boundary_loaded(mesh_geometry_);
    }
    else
    {
        emit this->warning("Could not open file!");
    }
}
