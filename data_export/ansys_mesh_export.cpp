#include "ansys_mesh_export.h"

void add_tetrahedra(graph &g, const vector4ui &vertices)
{
    g.add_connection(vertices[0], vertices[1]);
    g.add_connection(vertices[0], vertices[2]);
    g.add_connection(vertices[0], vertices[3]);
    g.add_connection(vertices[1], vertices[2]);
    g.add_connection(vertices[1], vertices[3]);
    g.add_connection(vertices[2], vertices[3]);
}

void add_pyramid(graph &g, const vector5ui &vertices)
{
    g.add_connection(vertices[0], vertices[1]);
    g.add_connection(vertices[0], vertices[3]);
    g.add_connection(vertices[0], vertices[4]);
    g.add_connection(vertices[1], vertices[2]);
    g.add_connection(vertices[1], vertices[4]);
    g.add_connection(vertices[2], vertices[3]);
    g.add_connection(vertices[2], vertices[4]);
    g.add_connection(vertices[3], vertices[4]);
}

void add_wedge(graph &g, const vector6ui &vertices)
{
    g.add_connection(vertices[0], vertices[1]);
    g.add_connection(vertices[0], vertices[2]);
    g.add_connection(vertices[0], vertices[3]);
    g.add_connection(vertices[1], vertices[2]);
    g.add_connection(vertices[1], vertices[4]);
    g.add_connection(vertices[2], vertices[5]);
    g.add_connection(vertices[3], vertices[4]);
    g.add_connection(vertices[3], vertices[5]);
    g.add_connection(vertices[4], vertices[5]);
}

void add_hexahedra(graph &g, const vector8ui &vertices)
{
    g.add_connection(vertices[0], vertices[1]);
    g.add_connection(vertices[0], vertices[2]);
    g.add_connection(vertices[0], vertices[4]);
    g.add_connection(vertices[1], vertices[3]);
    g.add_connection(vertices[1], vertices[5]);
    g.add_connection(vertices[2], vertices[3]);
    g.add_connection(vertices[2], vertices[6]);
    g.add_connection(vertices[3], vertices[7]);
    g.add_connection(vertices[4], vertices[5]);
    g.add_connection(vertices[4], vertices[6]);
    g.add_connection(vertices[5], vertices[7]);
    g.add_connection(vertices[6], vertices[7]);
}

void add_triangle(graph &g, const vector3ui &vertices)
{
    g.add_connection(vertices[0], vertices[1]);
    g.add_connection(vertices[0], vertices[2]);
    g.add_connection(vertices[1], vertices[2]);
}

void add_square(graph &g, const vector4ui &vertices)
{
    g.add_connection(vertices[0], vertices[1]);
    g.add_connection(vertices[0], vertices[3]);
    g.add_connection(vertices[1], vertices[2]);
    g.add_connection(vertices[2], vertices[3]);
}
