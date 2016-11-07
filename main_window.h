#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "application_data/application_data.h"

class Observer;
class QSplitter;
class QProgressBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

public slots:
    void open_file_action();

private:
    QProgressBar* progress_bar_;

    /**
     * Contains all application data
     */
    application_data* app_data_;

    QSplitter* splitter_;

    QStatusBar* status_bar_;
};

#endif // MAIN_WINDOW_H
