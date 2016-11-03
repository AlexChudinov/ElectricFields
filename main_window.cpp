#include "main_window.h"
#include <QToolBar>
#include <QAction>
#include <QSplitter>
#include <QBoxLayout>
#include <QStatusBar>
#include <QProgressBar>
#include "data_export/load_data_tread.h"

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

    //Magnify mesh view action
    QAction* magnify_action = new QAction(QIcon(":/Icons/magnify_icon"), "Magnify mesh", this);
    //Set default mesh position action
    QAction* default_view_action = new QAction(QIcon(":/Icons/default_view"), "Reset default view", this);

    //Create file tool bar and add actions to it
    QToolBar* file_toolbar = new QToolBar("File toolbar",this);
    this->addToolBar(file_toolbar);
    file_toolbar->addAction(file_open_action);

    //Create mesh view toolbar
    QToolBar* mesh_view_toolbar = new QToolBar("Mesh view toolbar", this);
    this->addToolBar(mesh_view_toolbar);
    mesh_view_toolbar->addAction(magnify_action);
    mesh_view_toolbar->addAction(default_view_action);

    gl_mesh_widget* gl_mesh_widget_ = new gl_mesh_widget(this);
    connect(this, SIGNAL(mesh_loaded(const mesh_geom*)),
            gl_mesh_widget_, SLOT(set_mesh_pointer(const mesh_geom*)));
    connect(magnify_action, SIGNAL(triggered()), gl_mesh_widget_, SLOT(magnify()));
    connect(default_view_action, SIGNAL(triggered()), gl_mesh_widget_, SLOT(set_default_view()));

    splitter_->addWidget(gl_mesh_widget_);
}

MainWindow::~MainWindow(){}

void MainWindow::open_file_action()
{
    QProgressBar progress_bar_;
    progress_bar_.setRange(0, 100);
    progress_bar_.setTextVisible(true);
    progress_bar_.setFormat("File loading...");
    progress_bar_.setAlignment(Qt::AlignCenter);
    status_bar_->addWidget(&progress_bar_);

    load_data_thread* load_thread = new load_data_thread(&progress_bar_, app_data_, this);
    connect(load_thread,SIGNAL(started()),this,SLOT());

    load_thread.run();

    status_bar_->removeWidget(&progress_bar_);
}
