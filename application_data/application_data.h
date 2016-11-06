#ifndef APP_DATA_H
#define APP_DATA_H

#include <QThread>
#include <QObject>
#include <QString>
#include <QRunnable>

#include "mesh_math/mesh_geometry.h"

using mesh_geom = mesh_geometry<float, uint32_t>;

class application_data : public QThread
{
    Q_OBJECT

public:
    enum PROCESS_ID
    {
        LOAD_MESH
    };

    application_data(QObject* parent = 0);
    virtual ~application_data();

    void run();

    void set_file_name(const QString& file_name)
    { file_name_ = file_name; }

    void set_process(PROCESS_ID process_id);

signals:
    void mesh_loaded(const mesh_geom*);
    void progress_value(int val);

public slots:
    void load_mesh(QString fileName);

private:
    void clear_mesh_();
    mesh_geom* mesh_geometry_;

    QString file_name_;
    void (application_data::*process_)(QString);
};

#endif // APP_DATA_H
