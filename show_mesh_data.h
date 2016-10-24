#ifndef SHOW_MESH_DATA_H
#define SHOW_MESH_DATA_H

#include <QOpenGLFunctions>
#include "mesh_geometry.h"

class QOpenGLBuffer;
class QOpenGLShaderProgram;

class show_mesh_geometry : public QOpenGLFunctions
{
    using mesh_geometry = mesh_geometry<float, uint32_t>;
    const mesh_geometry& geom_;

public:
    show_mesh_geometry(const mesh_geometry& geom);
    virtual ~show_mesh_geometry();

    void draw_mesh_geometry(QOpenGLShaderProgram* program);
private:
    void init_mesh_geometry_() const;

    QOpenGLBuffer* array_buf_;
};

#endif // SHOW_MESH_DATA_H
