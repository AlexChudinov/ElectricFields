#ifndef MESH_GEOMETRY_H
#define MESH_GEOMETRY_H

#include <graph.h>
#include <vector>
#include <vectortemplate.h>
#include <utility>
#include <map>

template<typename Float, typename label> class mesh_geometry;

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
    using boundary_regions_list = std::map<std::string, graph>;

    /**
     *  Node types of a mesh
     */
    enum NODE_TYPE : uint8_t
    {
        INNER_POINT = 0x00,
        BOUNDARY_ZERO_GRADIENT = 0x01,
        BOUNDARY_FIXED_VALUE = 0x02
    };
    using node_types_list= std::vector<NODE_TYPE>;

protected:
    graph mesh_connectivity_;
    node_positions node_positions_;
    boundary_regions_list boundary_mesh_;
    node_types_list node_types_;

public:
    mesh_geometry(const graph& g, const node_positions& np)
        : mesh_connectivity_(g),
          node_positions_(np),
          node_types_(np.size(), INNER_POINT)
    { assert(g.size() == np.size()); }

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
     * Returns a number of nodes in the mesh
     */
    size_t nodes_number() const { return node_positions_.size(); }

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

    /**
     * Checks if the boundary could be attributed to the mesh
     */
    bool check_boundary(const graph& mesh) const
    {
        return mesh.size() == this->nodes_number();
    }

    /**
     * Adds new boundary returns true if it is ok
     */
    bool add_boundary(const std::string& name, const graph& mesh, NODE_TYPE type = BOUNDARY_FIXED_VALUE)
    {
        //If the name already exists
        if(*(this->boundary_mesh_.lower_bound(name)) == name ) return false;

        //If the mesh is not fitted with the boundary
        if(check_boundary(mesh)) return false;

        boundary_mesh_[name] = mesh;

        for(size_t i = 0; i < mesh.size(); ++i)
        {
            node_types_[i] = type;
            for(const label& l : mesh.get_node_neighbour(i));
        }

        return true;
    }
};

#endif // MESH_GEOMETRY_H
