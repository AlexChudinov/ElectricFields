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
    this->connections_number_ = connections.size();
    this->array_buf_->bind();
    this->array_buf_->allocate
            (
                connections.data(),
                connections.size()*sizeof(mesh_geom::vector3f));
    this->set_defaults();
}

void mesh_geometry_engine::set_defaults()
{
    //Set image box parameters
    bounding_box_type box = geom_.containing_box();
    this->r0_ = .5 * QVector3D(
            box.second[0] + box.first[0],
            box.second[1] + box.first[1],
            box.second[2] + box.first[2]);
    this->scale_ = std::max(
                box.second[0] - box.first[0],
                std::max(
                    box.second[1] - box.first[1],
                    box.second[2] - box.first[2]));
}

gl_mesh_widget::gl_mesh_widget(QWidget *parent)
    :
      QOpenGLWidget(parent),
      program_(new QOpenGLShaderProgram(this)),
      mesh_geometry_(nullptr),
      rotation_(),
      translation_(0.0f, 0.0f)
{ }

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
    this->screen_width_ = w;
    this->screen_height_ = h > 0 ? h : 1;
}

void gl_mesh_widget::paintGL()
{
    if(mesh_geometry_)
    {
        float scale_factor = 2.0f/mesh_geometry_->scale();
        QMatrix4x4 matrix_modelview, matrix_projection, matrix_rotation;

        //Set box position, rotate and shift
        matrix_modelview.scale(scale_factor);
        matrix_modelview.translate(-mesh_geometry_->r0());
        matrix_rotation.rotate(rotation_);
        matrix_modelview = matrix_rotation*matrix_modelview;

        //Translate to z = -4.0
        matrix_modelview(2,3) -= 4.0;

        matrix_projection.perspective(
            45.0f,
            qreal(this->screen_width_)/qreal(this->screen_height_),
            2.0,
            6.0);

        matrix_modelview = matrix_projection * matrix_modelview;
        matrix_modelview(0,3) += translation_.x();
        matrix_modelview(1,3) += translation_.y();

        program_->setUniformValue("mvp_matrix", matrix_modelview);
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

void gl_mesh_widget::mouseMoveEvent(QMouseEvent *event)
{
    //Get position difference and normalize
    QVector2D diff = QVector2D(event->localPos()) - this->press_mouse_position_;
    diff.setX(diff.x() / this->width()); diff.setY(diff.y() / this->height());

    if(event->buttons() == Qt::LeftButton) //rotate
    {
        this->press_mouse_position_ = QVector2D(event->localPos());
        float rotation_angle = diff.length() != 0.0f ? 45.0f * diff.length() : 0.0f;
        QVector3D rotation_axis = QVector3D(diff.y(), diff.x(), 0.0f).normalized();
        rotation_ =
                QQuaternion::fromAxisAndAngle(rotation_axis, rotation_angle) * rotation_;
        this->update();
    }
    if(event->buttons() == Qt::RightButton) //shift
    {
        this->press_mouse_position_ = QVector2D(event->localPos());
        QVector2D shift(diff.x(), -diff.y());
        this->translation_ += shift;
        this->update();
    }
}

void gl_mesh_widget::set_default_view()
{
    if(this->mesh_geometry_)
    {
        this->rotation_ = QQuaternion();
        this->translation_ = QVector2D(0.0f, 0.0f);
        this->mesh_geometry_->set_defaults();
        this->update();
    }
}
