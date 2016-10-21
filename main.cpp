#include <QCoreApplication>
#include <graph.h>
#include <iostream>
#include <matrixtemplate.h>
#include <fstream>
#include <ansys_mesh_export.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::ifstream in;
    in.open("C:\\Proj_22022016\\ElectricFields\\test_quad_flat.geom");

    if(!in)
    {
        std::cout << "Could not open test file" << std::endl;
        return 1;
    }

    mesh_geometry<float, uint32_t> mg = parse_ansys_mesh<float, uint32_t>(in);
    in.close();

    return a.exec();
}
