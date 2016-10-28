#include "show_mesh_data.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QWheelEvent>

mesh_geometry_engine::mesh_geometry_engine(const mesh_geom &geom)
    : geom_(geom),
      array_buf_(new QOpenGLBuffer()),
      connections_number_(0)
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
    glDrawArrays(GL_LINES, 0, connections_number_);
}

void mesh_geometry_engine::init_mesh_geometry_()
{
    mesh_geom::node_positions connections = geom_.mesh_connections();
    connections_number_ = connections.size();
    array_buf_->bind();
    array_buf_->allocate
            (
                connections.data(),
                connections.size()*sizeof(mesh_geom::vector3f));

    //Set image box parameters
    bounding_box_type box = geom_.containing_box();
    r0_ = .5 * QVector3D(
            box.second[0] + box.first[0],
            box.second[1] + box.first[1],
            box.second[2] + box.first[2]);
    scale_ = std::max(
                box.second[0] - box.first[0],
                std::max(
                    box.second[1] - box.first[1],
                    box.second[2] - box.first[2]));
}

gl_mesh_widget::gl_mesh_widget(QWidget *parent)
    :
      QOpenGLWidget(parent),
      program_(new QOpenGLShaderProgram(this)),
      mesh_geometry_(nullptr)
{ rotation_matrix_.setToIdentity(); }

gl_mesh_widget::~gl_mesh_widget()
{
    this->makeCurrent();
    if(mesh_geometry_) delete mesh_geometry_;
    this->doneCurrent();
}

void gl_mesh_widget::initializeGL()
{
    this->initializeOpenGLFunctions();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    program_->addShaderFromSourceFile(QOpenGLShader::Vertex,":/Shaders/vector_shader");
    program_->addShaderFromSourceFile(QOpenGLShader::Fragment,":/Shaders/fragment_shader");
    program_->link();
    program_->bind();

    glEnable(GL_DEPTH_TEST);
}

void gl_mesh_widget::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 2.0, zFar = 6.0, fov = 45.0;
    projection_matrix_.setToIdentity();
    projection_matrix_.perspective(fov, aspect, zNear, zFar);
}

void gl_mesh_widget::paintGL()
{
    if(mesh_geometry_)
    {
        QMatrix4x4 matrix;
        matrix.setToIdentity();

        //Set scale
        matrix.scale(
            2.0f/mesh_geometry_->scale(),
            2.0f/mesh_geometry_->scale(),
            2.0f/mesh_geometry_->scale());
        //Set box position
        matrix.translate(
            -mesh_geometry_->r0().x(),
            -mesh_geometry_->r0().y(),
            mesh_geometry_->r0().z());
        matrix = matrix*rotation_matrix_;

        matrix.translate(0.0f, 0.0f, -4.0f);
        matrix.rotate(180.0f, 1.0f, 0.0f);

        program_->setUniformValue("mvp_matrix", projection_matrix_*matrix);
        mesh_geometry_->draw_mesh_geometry(program_);
    }
}

void gl_mesh_widget::set_mesh_pointer(const mesh_geom *geom)
{
    mesh_geometry_ = new mesh_geometry_engine(*geom);
    this->update();
}

void gl_mesh_widget::magnify(float factor)
{
    if(mesh_geometry_)
    {
        mesh_geometry_->scale() /= factor;
        this->update();
    }
}

void gl_mesh_widget::mousePressEvent(QMouseEvent *event)
{
    press_mouse_position_ = QVector2D(event->localPos());
}

void gl_mesh_widget::mouseReleaseEvent(QMouseEvent *event)
{
    QVector2D diff = QVector2D(event->localPos()) - this->press_mouse_position_;
    diff.setX(diff.x() / this->width()); diff.setY(diff.y() / this->height());

    float rotation_angle = diff.length() != 0.0f ? 180.0f/3.14f * atan(diff.length()/8.0f) : 0.0f;

    QVector3D rotation_axis = QVector3D(diff.y(), -diff.x(), 0.0f).normalized();

    rotation_matrix_.rotate(rotation_angle, rotation_axis);

    this->update();
}
