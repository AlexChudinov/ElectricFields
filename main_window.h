#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "application_data/application_data.h"

class Observer;
class QSplitter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    /**
     * Loading mesh data
     */
    void load_mesh_(const QString& file_name);

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

signals:
    void mesh_loaded(const mesh_geom*);

public slots:
    void open_file_action();

private:
    Observer* observer_;

    /**
     * Contains all application data
     */
    application_data* app_data_;

    QSplitter* splitter_;

    QStatusBar* status_bar_;
};

#endif // MAIN_WINDOW_H
