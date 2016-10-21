#include <QCoreApplication>
#include <graph.h>
#include <iostream>
#include <matrixtemplate.h>
#include <fstream>
#include <ansys_mesh_export.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    using matrix4x4 = math::matrix_c<double, 4, 4>;
    using vector4 = math::vector_c<double, 4>;

    matrix4x4 m{{1, 2 , 3, 4}, {4, 3, 2, 1},
                {2, 3, 2, 6}, {4, 5, 6, 4}};
    vector4 v{1, 0 , 0, 4};

    double det = math::det(m);

    std::cout << math::solve<double, 4, 1>(m,v,det) << std::endl;

    matrix4x4 m1{{1.,0.,0.,0.},{0.,1.,0.,0.},{0.,0.,2.,0.},{0.,0.,0.,1}};
    std::cout << m1 << std::endl;
    std::cout << math::det(m1) << std::endl;

    data_structs::graph<unsigned long> gr;

    gr.add_connection(0,1);
    gr.add_connection(0,2);

    std::ifstream in;
    in.open("test_field.geom");
    mesh_geometry<float, uint32_t> mg = parse_ansys_mesh<float, uint32_t>(in);
    in.close();

    return a.exec();
}
