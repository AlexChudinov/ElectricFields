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

    /**
     * Shows message box on a screen if one of the threads emits wwarnings or whatever
     */
    void show_message_box(QString msg);

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
