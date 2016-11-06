#include "main_window.h"
#include "observer.h"
#include "actionslist.h"
#include "display_mesh/show_mesh_data.h"

#include <QThreadPool>
#include <QToolBar>
#include <QAction>
#include <QSplitter>
#include <QStatusBar>
#include <QFileDialog>
#include <QProgressBar>

void MainWindow::load_mesh_(const QString& file_name)
{
    app_data_->set_process(application_data::LOAD_MESH);
    app_data_->set_file_name(file_name);
    actions_list[FILE_OPEN_ACTION_ID]->setEnabled(false);
    connect(app_data_, SIGNAL(progress_value(int)),
            observer_, SLOT(setProgressVal(int)));
    connect(app_data_, SIGNAL(mesh_loaded(const mesh_geom*)),
            this, SIGNAL(mesh_loaded(const mesh_geom*)));
    connect(app_data_, SIGNAL(finished()),
            this->observer_, SLOT(hideProgressBar()));
    connect(this->observer_, SIGNAL(hiden(bool)),
            actions_list[FILE_OPEN_ACTION_ID], SLOT(setEnabled(bool)));
    app_data_->start();
}

MainWindow::MainWindow(QWidget *parent)
    :
      QMainWindow(parent),
      observer_(new Observer(this)),
      app_data_(new application_data(this)),
      splitter_(new QSplitter(this)),
      status_bar_(new QStatusBar(this))
{
    //Set widgets and parameters
    this->resize(600, 400);
    this->setCentralWidget(splitter_);
    this->setStatusBar(status_bar_);

    //File open action:
    actions_list.push_back(new QAction(QIcon(":/Icons/file_open_icon"),"Open file",this));
    connect(actions_list[FILE_OPEN_ACTION_ID],SIGNAL(triggered()),
            this,SLOT(open_file_action()));

    //Magnify mesh view action
    QAction* magnify_action = new QAction(QIcon(":/Icons/magnify_icon"), "Magnify mesh", this);
    //Set default mesh position action
    QAction* default_view_action = new QAction(QIcon(":/Icons/default_view"), "Reset default view", this);

    //Create file tool bar and add actions to it
    QToolBar* file_toolbar = new QToolBar("File toolbar",this);
    this->addToolBar(file_toolbar);
    file_toolbar->addAction(actions_list[FILE_OPEN_ACTION_ID]);

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
    this->status_bar_->addWidget(this->observer_->progressBar());
    this->observer_->progressBar()->show();
    this->observer_->initProgress("File loading...");
    QString file_name = QFileDialog::getOpenFileName
            (
                this,
                "Open file dialog",
                QString(),
                "Ansys mesh: (*.geom)");
    if( file_name.isNull() ) return;

    if( file_name.split(".").at(1) == "geom" )
        this->load_mesh_(file_name);
}
