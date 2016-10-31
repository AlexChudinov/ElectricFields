#ifndef SHOW_MESH_DATA_H
#define SHOW_MESH_DATA_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QVector2D>
#include <QQuaternion>
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

    /**
     * Returns central point and scale of a geometry
     */
    inline const QVector3D& r0() const { return r0_; }
    inline float scale() const { return scale_; }
    inline QVector3D& r0() { return r0_; }
    inline float& scale() { return scale_; }

    /**
     * Sets default image box parameters
     */
    void set_defaults();

private:
    void init_mesh_geometry_();

    QOpenGLBuffer* array_buf_;

    size_t connections_number_;

    QVector3D r0_; //Central point

    float scale_;
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

    void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
public slots:
    void set_mesh_pointer(const mesh_geom *geom);
    void magnify(float factor = 1.1);

    /**
     * Returns default position of a picture onto screen
     */
    void set_default_view();

private:
    QOpenGLShaderProgram* program_;

    mesh_geometry_engine* mesh_geometry_;

    QVector2D press_mouse_position_;

    QQuaternion rotation_;

    int screen_width_, screen_height_;
};

#endif // SHOW_MESH_DATA_H
