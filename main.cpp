#include <QCoreApplication>
#include <quad_3d_fun.h>
#include <iostream>

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

    using math::quad_function_matrix;
    using points_type = math::matrix_c<double, 12, 3>;

    points_type pts{
        {0.,0.,0.}, //1
        {0.,0.,1.}, //2
        {0.,0.,-1.}, //3
        {0.,1.,0.}, //4
        {0.,-1,0.}, //5
        {1.,0.,0.}, //6
        {-1.,0.,0.},//7
        {-1.,-1.,0.},//8
        {1.,1.,0.},  //9
        {1.,-1.,0.}, //10
        {-1.,1.,0.}, //11
        {0.,1.,1.} //12
    };

    quad_function_matrix<double> qm(pts);

    std::cout << pts << std::endl;
    std::cout << qm.quad_matrix_ << std::endl;

    return a.exec();
}
