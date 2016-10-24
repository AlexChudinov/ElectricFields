#include "show_mesh_data.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

show_mesh_geometry::show_mesh_geometry(const mesh_geometry &geom)
    : geom_(geom),
      array_buf_(new QOpenGLBuffer)
{
    this->initializeOpenGLFunctions();
    array_buf_->create();
    this->init_mesh_geometry_();
}

show_mesh_geometry::~show_mesh_geometry()
{
    array_buf_->destroy();
    delete array_buf_;
}

void show_mesh_geometry::draw_mesh_geometry(QOpenGLShaderProgram *program)
{
    array_buf_->bind();
    int variable_location = program->attributeLocation("vertex_position");
    program->enableAttributeArray(variable_location);
    program->setAttributeBuffer(variable_location, GL_FLOAT, 0, 3);
    glDrawArrays(GL_LINES, 0, 3);
}

void show_mesh_geometry::init_mesh_geometry_() const
{
    mesh_geometry::node_positions connections = geom_.mesh_connections();
    array_buf_->allocate
            (
                connections.data(),
                connections.size()*sizeof(mesh_geometry::vector3f));
}
