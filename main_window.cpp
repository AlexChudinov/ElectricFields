#include "main_window.h"
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QVBoxLayout>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent)
    :
      QMainWindow(parent),
      tool_bar_(new QToolBar(this)),
      file_open_action_(new QAction(QIcon(":/file_open_icon"),QObject::tr("Open file"), parent)),
      app_data_(),
      central_widget_(new QWidget(this)),
      window_layout_(new QVBoxLayout(this)),
      boundary_regions_(new QTableWidget(this))
{
    this->addToolBar(tool_bar_);
    tool_bar_->addAction(file_open_action_);
    this->setCentralWidget(central_widget_);
    central_widget_->setLayout(window_layout_);
    window_layout_->addWidget(boundary_regions_);

    //Do connections
    connect(file_open_action_,SIGNAL(triggered()),
            this,SLOT(open_file_action()));
}

MainWindow::~MainWindow(){}

void MainWindow::open_file_action()
{
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
            app_data_.mesh_geometry_
                    = new app_data::mesh_geom
                    (parse_ansys_mesh<float, uint32_t>
                     (
                         stream,
                         [](QTextStream& stream){stream.readLine();}
                     ));
            file.close();
        }
    }
}
