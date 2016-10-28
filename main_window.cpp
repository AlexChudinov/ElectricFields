#include "main_window.h"
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QSplitter>
#include <QBoxLayout>
#include <QStatusBar>
#include <QProgressBar>

MainWindow::MainWindow(QWidget *parent)
    :
      QMainWindow(parent),
      status_bar_(new QStatusBar(this)),
      app_data_(),
      splitter_(new QSplitter(this))
{
    //Set widgets and parameters
    this->resize(600, 400);
    this->setCentralWidget(splitter_);
    this->setStatusBar(status_bar_);

    //File open action:
    QAction* file_open_action = new QAction(QIcon(":/Icons/file_open_icon"),"Open file",this);
    connect(file_open_action,SIGNAL(triggered()),
            this,SLOT(open_file_action()));

    //Magnify mesh view toolbar
    QAction* magnify_action = new QAction(QIcon(":/Icons/magnify_icon"), "Magnify mesh", this);

    //Create file tool bar and add actions to it
    QToolBar* file_toolbar = new QToolBar("File toolbar",this);
    this->addToolBar(file_toolbar);
    file_toolbar->addAction(file_open_action);

    //Create mesh view toolbar
    QToolBar* mesh_view_toolbar = new QToolBar("Mesh view toolbar", this);
    this->addToolBar(mesh_view_toolbar);
    mesh_view_toolbar->addAction(magnify_action);

    gl_mesh_widget* gl_mesh_widget_ = new gl_mesh_widget(this);
    connect(this, SIGNAL(mesh_loaded(const mesh_geom*)),
            gl_mesh_widget_, SLOT(set_mesh_pointer(const mesh_geom*)));
    connect(magnify_action, SIGNAL(triggered()), gl_mesh_widget_, SLOT(magnify()));
    splitter_->addWidget(gl_mesh_widget_);
}

MainWindow::~MainWindow(){}

void MainWindow::open_file_action()
{
    QProgressBar progress_bar_;
    progress_bar_.setRange(0, 5);
    progress_bar_.setTextVisible(true);
    progress_bar_.setFormat("File loading...");
    progress_bar_.setAlignment(Qt::AlignCenter);
    status_bar_->addWidget(&progress_bar_);

    QString file_name = QFileDialog::getOpenFileName
            (
                this,
                "Open file",
                QString(),
                QString(MainWindow::file_open_filters_));

    if(!file_name.isNull())
    {
        QFile file(file_name);
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning
                    (this,
                     "File open dialog",
                     QString("Fail to open file: ").append(file_name));
            return;
        }
        else
        {
            QTextStream stream(&file);
            int progress = 1;
            app_data_.mesh_geometry_
                    = new app_data::mesh_geom
                    (parse_ansys_mesh<float, uint32_t>
                     (
                         stream,
                         [&progress_bar_, &progress](QTextStream& stream)
            {
                         QString name = stream.readLine();
                         progress_bar_.setFormat(name);
                         progress_bar_.setValue(++progress);
            }
                     ));
            file.close();
            emit mesh_loaded(app_data_.mesh_geometry_);
        }
    }

    status_bar_->removeWidget(&progress_bar_);
}
