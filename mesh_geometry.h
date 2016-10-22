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
        //Count all connections
        size_t connection_number = 0;
        for(const typename graph::label_vector_type& lv : mesh_connectivity_)
            connection_number += lv.size();

        //Read all connections
        //Note: number of points is double number of connections
        //Every connection was counted twice
        node_positions res(connection_number);
        std::vector<bool> visited(mesh_connectivity_.size(),false);
        connection_number = 0;
        for(size_t i = 0; i < mesh_connectivity_.size(); ++i)
        {
            visited[i] = true;
            for(size_t j = 0; j < mesh_connectivity_[i].size(); ++j)
            {
                if(!visited[j])
                {
                    res[connection_number++] = node_positions_[i];
                    res[connection_number++]
                            = node_positions_[mesh_connectivity_[i][j]];
                }
            }
        }

        return res;
    }
};

#endif // MESH_GEOMETRY_H
