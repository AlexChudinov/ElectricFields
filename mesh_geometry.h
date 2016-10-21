#ifndef MESH_GEOMETRY_H
#define MESH_GEOMETRY_H

#include <graph.h>
#include <vector>
#include <vectortemplate.h>

/**
 * Mesh connectivity and node space positions
 */
template<typename Float, typename label>
class mesh_geometry
{
public:
    using graph          = data_structs::graph<label>;
    using vector3f       = math::vector_c<Float, 3>;
    using node_positions = std::vector<vector3f>;

private:
    graph mesh_connectivity_;
    node_positions node_positions_;

public:
    mesh_geometry(const graph& g, const node_positions& np)
        : mesh_connectivity_(g), node_positions_(np)
    { assert(mesh_connectivity_.size() == node_positions_.size()); }
};

#endif // MESH_GEOMETRY_H
