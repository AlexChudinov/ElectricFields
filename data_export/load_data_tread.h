#ifndef LOAD_DATA_TREAD_H
#define LOAD_DATA_TREAD_H

#include <QThread>
#include <app_data.h>

class QProgressBar;
class QFileDialog;

/**
 * This class is responsible for the data loading
 */
class load_data_thread : public QThread
{
    Q_OBJECT

public:
    load_data_thread(
            QProgressBar* progress_bar,
            app_data* data,
            QObject* parent = 0);
    ~load_data_thread();

    void run();


signals:

public slots:

private slots:

private:
    QProgressBar* progress_bar_;
    app_data* data_;
};

#endif // LOAD_DATA_TREAD_H
