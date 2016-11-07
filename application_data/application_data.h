#ifndef APP_DATA_H
#define APP_DATA_H

#include <QThread>
#include <QObject>
#include <QString>
#include <QRunnable>
#include <QSharedPointer>

#include "mesh_math/mesh_geometry.h"

using mesh_geom = mesh_geometry<float, uint32_t>;

/**
 * Keeps application data and starts all data processes in a separate thread
 */
class application_data : public QThread
{
    Q_OBJECT

public:
    /**
     * Processes that can be run in a separate thread
     */
    enum PROCESS_ID
    {
        LOAD_MESH ///Load mesh from a file
    };

    application_data(QObject* parent = 0);
    virtual ~application_data(){}

    void run();

    /**
     * Sets the file name for data
     */
    void set_file_name(const QString& file_name)
    { file_name_ = file_name; }

    /**
     * Choose which process to run in a separate thread
     * Note, the processes description is in enum PROCESS_ID
     */
    void set_process(PROCESS_ID process_id);

signals:
    /**
     * new mesh was loaded
     */
    void mesh_loaded(const QSharedPointer<mesh_geom>&);

    /**
     * Progress flow indicator
     */
    void progress_value(int val);

    /**
     * QAction setEnable(bool) support
     */
    void application_data_holder_busy(bool is_busy = true);
    void application_data_holder_free(bool is_free = true);

public slots:
    /**
     * Load mesh from the file file_name
     */
    void load_mesh(QString file_name);

private:
    QSharedPointer<mesh_geom> mesh_geometry_;

    QString file_name_;
    /**
     * Current process to run into a separate thread
     */
    void (application_data::*process_)(QString);
};

#endif // APP_DATA_H
