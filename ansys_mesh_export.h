#ifndef ANSYS_MESH_LOAD_H
#define ANSYS_MESH_LOAD_H

#include <sstream>
#include <graph.h>
#include <cstddef>
#include <vectortemplate.h>

using graph     = data_structs::graph<uint32_t>;
using vector4ui = math::vector_c<uint32_t, 4>;
using vector5ui = math::vector_c<uint32_t, 5>;
using vector6ui = math::vector_c<uint32_t, 6>;
using vector8ui = math::vector_c<uint32_t, 8>;

/**
 * Loads ANSYS mesh from a file
 */
void parse_ansys_mesh(std::istream& in);

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

#endif // ANSYS_MESH_LOAD_H
