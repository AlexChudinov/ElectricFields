#ifndef ANSYS_MESH_LOAD_H
#define ANSYS_MESH_LOAD_H

#include <sstream>
#include <cstddef>
#include <string>

#include "mesh_math/mesh_geometry.h"
#include "linear_algebra/vectortemplate.h"

using graph     = data_structs::graph<uint32_t>;
using mesh_geom = mesh_geometry<float, uint32_t>;
using vector3ui = math::vector_c<uint32_t, 3>;
using vector4ui = math::vector_c<uint32_t, 4>;
using vector5ui = math::vector_c<uint32_t, 5>;
using vector6ui = math::vector_c<uint32_t, 6>;
using vector8ui = math::vector_c<uint32_t, 8>;

/**
 * Adds Ansys formated tetrahedra connectivity to a graph
 */
void add_tetrahedra(graph& g, const vector4ui& vertices);

/**
 * Adds Ansys formated pyramid connectivity to a graph
 */
void add_pyramid(graph& g, const vector5ui& vertices);

/**
 * Adds Ansys formated wedge connectivity to a graph
 */
void add_wedge(graph& g, const vector6ui& vertices);

/**
 * Adds Ansys formated hexaherdral connectivity to a graph
 */
void add_hexahedra(graph& g, const vector8ui& vertices);

/**
 * Adds Ansys formated triangle region connectivity to a graph
 */
void add_triangle(graph& g, const vector3ui& vertices);

/**
 * Adds Ansys formated square region connectivity to a graph
 */
void add_square(graph& g, const vector4ui& vertices);

/**
 * Loads ANSYS mesh from a file
 */
template<typename Stream, typename SkipLine>
mesh_geom parse_ansys_mesh
(
        Stream &in,
        SkipLine skip_line)
{
    size_t elems_number;

    skip_line(in); //Read word "Coordinates"
    in >> elems_number;
    typename mesh_geom::node_positions nodes(elems_number);
    typename mesh_geom::node_positions::iterator it = nodes.begin();
    for(; it != nodes.end(); ++it)
    {
        typename mesh_geom::vector3f& node = *it;
        in >> elems_number >> node[0] >> node[1] >> node[2];
    }
    skip_line(in);

    typename mesh_geom::graph gr;

    //Read tetrahedrals
    skip_line(in);
    in >> elems_number;
    for(size_t i = 0; i < elems_number; ++i)
    {
        vector4ui vertices;
        in >> vertices[0] >> vertices[1] >> vertices[2] >> vertices[3];
        vertices -= 1u;
        add_tetrahedra(gr,vertices);
    }
    skip_line(in);

    //Read pyramids
    skip_line(in);
    in >> elems_number;
    for(size_t i = 0; i < elems_number; ++i)
    {
        vector5ui vertices;
        in >> vertices[0] >> vertices[1] >> vertices[2] >> vertices[3] >> vertices[4];
        vertices -= 1u;
        add_pyramid(gr,vertices);
    }
    skip_line(in);

    //Read wedges
    skip_line(in);
    in >> elems_number;
    for(size_t i = 0; i < elems_number; ++i)
    {
        vector6ui vertices;
        in >> vertices[0] >> vertices[1] >> vertices[2] >> vertices[3] >> vertices[4] >> vertices[5];
        vertices -= 1u;
        add_wedge(gr,vertices);
    }
    skip_line(in);

    //Read hexahedrals
    skip_line(in);
    in >> elems_number;
    for(size_t i = 0; i < elems_number; ++i)
    {
        vector8ui vertices;
        in >>
                vertices[0] >> vertices[1] >> vertices[2] >>
                vertices[3] >> vertices[4] >> vertices[5] >>
                vertices[6] >> vertices[7];
        vertices -= 1u;
        add_hexahedra(gr,vertices);
    }

    return mesh_geom(gr, nodes);
}

/**
 * Loads mesh boundary regions from a file
 */
template<typename Stream, typename SkipLine>
bool parse_ansys_boundary_mesh
(
        mesh_geom& mesh,
        Stream &in,
        SkipLine skip_line)
{
    using string = std::string;

    size_t regions_number;

    in >> regions_number;
    for(size_t j = 0; j < regions_number; ++j)
    {
        skip_line();
        size_t elems_number;
        char name_buff[256];
        string boundary_name;
        graph boundary_connectivity;
        in >> name_buff;
        boundary_name = name_buff;
        in >> elems_number;
        //Read triangles
        for(size_t i = 0; i < elems_number; ++i)
        {
            vector3ui vertices;
            in >> vertices[0] >> vertices[1] >> vertices[2];
            vertices -= 1u;
            add_triangle(boundary_connectivity, vertices);
        }
        //Read squares
        in >> elems_number;
        for(size_t i = 0; i < elems_number; ++i)
        {
            vector4ui vertices;
            in >> vertices[0] >> vertices[1] >> vertices[2] >> vertices[3];
            vertices -= 1u;
            add_square(boundary_connectivity, vertices);
        }
        if (!mesh.add_boundary(boundary_name, boundary_connectivity)) return false;
    }
    return true;
}

#endif // ANSYS_MESH_LOAD_H
