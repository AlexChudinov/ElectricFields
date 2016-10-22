#include <QApplication>
#include <graph.h>
#include <iostream>
#include <matrixtemplate.h>
#include <fstream>
#include <ansys_mesh_export.h>
#include <main_window.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow main_window;
    main_window.show();

    /*std::ifstream in;
    in.open("C:\\Proj_22022016\\ElectricFields\\test_quad_flat.geom");

    if(!in)
    {
        std::cout << "Could not open test file" << std::endl;
        return 1;
    }

    mesh_geometry<float, uint32_t> mg = parse_ansys_mesh<float, uint32_t>(in);
    in.close();*/

    return a.exec();
}
