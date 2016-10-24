#ifndef MESH_GEOMETRY_H
#define MESH_GEOMETRY_H

#include <graph.h>
#include <vector>
#include <vectortemplate.h>
#include <utility>

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
    using box3D          = std::pair<vector3f, vector3f>;

    /**
     * Type of a boundary condition
     */
    enum BOUNDARY_REGION_TYPE
    {
        FIXED_VAL = 0x00,
        ZERO_GRAD = 0x01
    };

private:
    graph mesh_connectivity_;
    node_positions node_positions_;

public:
    mesh_geometry(const graph& g, const node_positions& np)
        : mesh_connectivity_(g), node_positions_(np)
    { assert(mesh_connectivity_.size() == node_positions_.size()); }

    /**
     * Returns all mesh connections for OpenGL drawing with GL_LINES flag
     */
    node_positions mesh_connections() const
    {
        node_positions res(mesh_connectivity_.connections_number()*2); //Each connection contains two vertices
        size_t counter = 0;

        auto observer = [&res, &counter, this](size_t i, size_t j)
        {
            res[counter++] = node_positions_[i];
            res[counter++] = node_positions_[j];
        };

        mesh_connectivity_.iterate_over_unique_connections(observer);

        return res;
    }

    /**
     * Returns minimal box that contains whole mesh
     */
    box3D containing_box() const
    {
        Float   min_x = node_positions_[0][0],
                min_y = node_positions_[0][1],
                min_z = node_positions_[0][2],
                max_x = node_positions_[0][0],
                max_y = node_positions_[0][1],
                max_z = node_positions_[0][2];

        typename node_positions::const_iterator it = node_positions_.begin() + 1;
        for(; it != node_positions_.end(); ++it)
        {
            min_x = std::min(min_x, (*it)[0]);
            min_y = std::min(min_y, (*it)[1]);
            min_z = std::min(min_z, (*it)[2]);
            max_x = std::max(max_x, (*it)[0]);
            max_y = std::max(max_y, (*it)[1]);
            max_z = std::max(max_z, (*it)[2]);
        }

        return box3D({min_x, min_y, min_z}, {max_x, max_y, max_z});
    }
};

#endif // MESH_GEOMETRY_H
