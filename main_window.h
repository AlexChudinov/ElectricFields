#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "show_mesh_data.h"
#include <app_data.h>

class QVBoxLayout;
class QTableWidget;
class QSplitter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

signals:
    void mesh_loaded(const mesh_geom* mesh);
    void status_widget(QWidget*);
    void status_remove_widget();

public slots:
    void open_file_action();

private:
    QStatusBar* status_bar_;

    constexpr static const char* file_open_filters_ = "Ansys mesh: (*.geom)";

    /**
     * Contains all application data
     */
    app_data app_data_;

    QSplitter* splitter_;
};

#endif // MAIN_WINDOW_H
