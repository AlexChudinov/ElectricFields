#include <load_data_tread.h>
#include <QProgressBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

load_data_thread::load_data_thread(QProgressBar *progress_bar,
        app_data *data,
        QObject *parent)
    :
      QThread(parent),
      progress_bar_(progress_bar),
      data_(data)
{
    assert(progress_bar && data && fun);
}

load_data_thread::~load_data_thread() {}

void load_data_thread::run()
{
    QString file_name = QFileDialog::getOpenFileName
            (
                0,
                "Open mesh data file",
                QString(),
                "Ansys mesh: (*.geom)");

    if(file_name.isNull()) return;

    if(file_name.split(".").at(1) == "geom")
    {
        QFile file(file_name);
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning
                    (this,
                     "File open dialog",
                     QString("Fail to open file: ").append(file_name).append("!"));
            return;
        }
        else
        {
            QTextStream stream(&file);
            int progress = 0;
            if(data_) delete data_;
            data_->mesh_geometry_
                    = new app_data::mesh_geom
                    (parse_ansys_mesh<float, uint32_t>
                     (
                         stream,
                         [progress_bar_, &progress](QTextStream& stream)
            {
                         stream.readLine();
                         progress_bar_->setValue(progress += 12);
            }));
            file.close();
        }
    }
}
