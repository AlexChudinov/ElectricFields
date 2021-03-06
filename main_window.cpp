#include "main_window.h"
#include "display_mesh/show_mesh_data.h"
#include "display_mesh/mesh_model.h"

#include <QThreadPool>
#include <QToolBar>
#include <QAction>
#include <QSplitter>
#include <QStatusBar>
#include <QFileDialog>
#include <QProgressBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    :
      QMainWindow(parent),
      progress_bar_(new QProgressBar(this)),
      app_data_(new application_data(this)),
      splitter_(new QSplitter(this)),
      status_bar_(new QStatusBar(this))
{
    //Initialize widgets and parameters
    this->resize(600, 400);
    this->setCentralWidget(splitter_);
    this->setStatusBar(status_bar_);
    progress_bar_->hide();
    connect(app_data_, SIGNAL(warning(QString)), this, SLOT(show_message_box(QString)));

    //Set status bar connections
    status_bar_->addWidget(progress_bar_);
    connect(app_data_, SIGNAL(finished()), progress_bar_, SLOT(hide()));
    connect(app_data_, SIGNAL(progress_value(int)), progress_bar_, SLOT(setValue(int)));

    //File open action:
    QAction* file_open_action = new QAction(QIcon(":/Icons/file_open_icon"),"Open file",this);
    connect(file_open_action, SIGNAL(triggered()), this, SLOT(open_file_action()));
    connect(app_data_, SIGNAL(application_data_holder_busy(bool)), file_open_action, SLOT(setDisabled(bool)));
    connect(app_data_, SIGNAL(application_data_holder_free(bool)), file_open_action, SLOT(setEnabled(bool)));

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

    //Create OpenGL context for mesh rendering
    gl_mesh_widget* gl_mesh_widget_ = new gl_mesh_widget(this);
    connect(app_data_, SIGNAL(mesh_loaded(const QSharedPointer<mesh_geom>&)),
            gl_mesh_widget_, SLOT(set_mesh(const QSharedPointer<mesh_geom>&)));
    connect(magnify_action, SIGNAL(triggered()), gl_mesh_widget_, SLOT(magnify()));
    connect(default_view_action, SIGNAL(triggered()), gl_mesh_widget_, SLOT(set_default_view()));

    //Create table view to show all mesh and field parameters
    mesh_model_view* mesh_model_view_ = new mesh_model_view(this);
    connect(app_data_, SIGNAL(mesh_loaded(QSharedPointer<mesh_geom>)),
            mesh_model_view_, SLOT(reset_mesh_model(QSharedPointer<mesh_geom>)));
    connect(app_data_, SIGNAL(boundary_loaded(const QSharedPointer<mesh_geom>&)),
            mesh_model_view_, SLOT(reset_mesh_model(const QSharedPointer<mesh_geom>&)));

    //Show mesh and boundary parameters table
    splitter_->addWidget(gl_mesh_widget_);
    splitter_->addWidget(mesh_model_view_);
    splitter_->setStretchFactor(0, 10);
    splitter_->setStretchFactor(1, 1);

}

MainWindow::~MainWindow(){}

void MainWindow::open_file_action()
{
    QString file_name = QFileDialog::getOpenFileName
            (
                this,
                "Open file dialog",
                QString(),
                "Ansys mesh: (*.geom) ;; Boundary regions: (*.rgn)");
    if( file_name.isNull() ) return;

    if( file_name.split(".").at(1) == "geom" )
    {
        progress_bar_->show();
        app_data_->set_process(application_data::LOAD_MESH);
        app_data_->set_file_name(file_name);
        app_data_->start();
        return;
    }

    if( file_name.split(".").at(1) == "rgn" )
    {
        progress_bar_->show();
        app_data_->set_process(application_data::LOAD_BOUNDARY);
        app_data_->set_file_name(file_name);
        app_data_->start();
        return;
    }

    QMessageBox::warning(0, "Open file dialog", "Unsupported file type");
}

void MainWindow::show_message_box(QString msg)
{
    QMessageBox::warning(0, "Main application warning", msg);
}
