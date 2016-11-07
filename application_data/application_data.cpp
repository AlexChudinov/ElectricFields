#include "application_data.h"
#include "data_export/ansys_mesh_export.h"

#include <QMessageBox>
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

