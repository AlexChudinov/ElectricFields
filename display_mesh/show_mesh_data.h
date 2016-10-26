#ifndef SHOW_MESH_DATA_H
#define SHOW_MESH_DATA_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include "mesh_geometry.h"
#include "show_mesh_data.h"

class QOpenGLBuffer;
class QOpenGLShaderProgram;

using mesh_geom         = mesh_geometry<float, uint32_t>;
using bounding_box_type = mesh_geom::box3D;

class mesh_geometry_engine : public QOpenGLFunctions
{
    const mesh_geom& geom_;

public:
    mesh_geometry_engine(const mesh_geom& geom);
    virtual ~mesh_geometry_engine();

    void draw_mesh_geometry(QOpenGLShaderProgram* program);

    const bounding_box_type& box() { return box_; }
private:
    void init_mesh_geometry_();

    QOpenGLBuffer* array_buf_;

    size_t connections_number_;

    bounding_box_type box_;
};

class gl_mesh_widget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    gl_mesh_widget(QWidget* parent = 0);
    ~gl_mesh_widget() Q_DECL_OVERRIDE;

    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;

public slots:
    void set_mesh_pointer(const mesh_geom *geom);

private:
    QOpenGLShaderProgram* program_;

    mesh_geometry_engine* geometry_;

    QMatrix4x4 projection_matrix_;
};

#endif // SHOW_MESH_DATA_H