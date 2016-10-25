#include "show_mesh_data.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

mesh_geometry_engine::mesh_geometry_engine(const mesh_geom &geom)
    : geom_(geom),
      array_buf_(new QOpenGLBuffer())
{
    this->initializeOpenGLFunctions();
    array_buf_->create();
    this->init_mesh_geometry_();
}

mesh_geometry_engine::~mesh_geometry_engine()
{
    array_buf_->destroy();
    delete array_buf_;
}

void mesh_geometry_engine::draw_mesh_geometry(QOpenGLShaderProgram *program)
{
    array_buf_->bind();
    int attr_location = program->attributeLocation("vertex_position");
    program->enableAttributeArray(attr_location);
    program->setAttributeBuffer(attr_location, GL_FLOAT, 0, 3);
    glDrawArrays(GL_LINES, 0, 3);
}

void mesh_geometry_engine::init_mesh_geometry_() const
{
    mesh_geom::node_positions connections = geom_.mesh_connections();
    array_buf_->allocate
            (
                connections.data(),
                connections.size()*sizeof(mesh_geom::vector3f));
}

gl_mesh_widget::gl_mesh_widget(QWidget *parent)
    :
      QOpenGLWidget(parent),
      program_(new QOpenGLShaderProgram(this)),
      geometry_(nullptr)
{}

gl_mesh_widget::~gl_mesh_widget()
{
    this->makeCurrent();
    if(geometry_) delete geometry_;
    this->doneCurrent();
}

void gl_mesh_widget::initializeGL()
{
    this->initializeOpenGLFunctions();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    program_->addShaderFromSourceFile(QOpenGLShader::Vertex,":/vshader.glsl");
    program_->link();
    program_->bind();

    glEnable(GL_DEPTH_TEST);
}

void gl_mesh_widget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection_matrix_.setToIdentity();

    // Set perspective projection
    projection_matrix_.perspective(fov, aspect, zNear, zFar);
}

void gl_mesh_widget::paintGL()
{
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.translate(0.0, 0.0, -4.0);

    program_->setUniformValue("mvp_matrix", projection_matrix_*matrix);

    if(geometry_)
    {
        geometry_->draw_mesh_geometry(program_);
    }
}

void gl_mesh_widget::set_mesh_pointer(const mesh_geom *geom)
{
    geometry_ = new mesh_geometry_engine(*geom);
    this->update();
}
