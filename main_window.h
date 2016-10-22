#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <app_data.h>

class QVBoxLayout;
class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

public slots:
    void open_file_action();

private:
    QToolBar* tool_bar_;

    constexpr static const char* file_open_filters_ = "Ansys mesh: (*.geom)";
    QAction* file_open_action_;

    /**
     * Contains all application data
     */
    app_data app_data_;

    /**
     * Puts widgets on window
     */
    QWidget* central_widget_;
    QVBoxLayout* window_layout_;
    QTableWidget* boundary_regions_;
};

#endif // MAIN_WINDOW_H
