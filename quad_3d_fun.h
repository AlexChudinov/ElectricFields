#ifndef QUAD_3D_FUN_H
#define QUAD_3D_FUN_H

#include <matrixtemplate.h>

/**
 * Defines quadratic function in 3D space
 * U(x,y,z) =   A x^2 + B y^2 + C z^2 + ...
 *              D xy  + E xz  + F yz  + ...
 *              G x   + H y   + I z   + ...
 *              J     + K     + L
 */
namespace math {

template<class Float>
struct quad_function_matrix
{
    using points_type = matrix_c<Float, 12, 3>;
    using values_type   = vector_c<Float, 12>;
    using matrix            = matrix_c<Float, 12, 12>;

    matrix quad_matrix_;
    Float  quad_matrix_det_;

    inline quad_function_matrix(const points_type& points)
    {
        using col_vector1 = proxy_matrix_col<Float, 12, 12>;
        using col_vector2 = const_proxy_matrix_col<Float, 12, 3>;

        auto set_values = [&points, this](size_t idx)->void
        {
            matrix& m = this->quad_matrix_;
            m[idx][0] = points[idx][0]*points[idx][0]; //x^2
            m[idx][1] = points[idx][1]*points[idx][1]; //y^2
            m[idx][2] = points[idx][2]*points[idx][2]; //z^2
            m[idx][3] = points[idx][0]*points[idx][1]; //xy
            m[idx][4] = points[idx][0]*points[idx][2]; //xz
            m[idx][5] = points[idx][1]*points[idx][2]; //yz
            m[idx][6] = points[idx][0]; //x
            m[idx][7] = points[idx][1]; //y
            m[idx][8] = points[idx][2]; //z
            m[idx][9] = m[idx][10] = m[idx][11] = 1.0; //free terms
        };

        math::For<0, 12, true>().Do(set_values);
    }
};

}

#endif // QUAD_3D_FUN_H
