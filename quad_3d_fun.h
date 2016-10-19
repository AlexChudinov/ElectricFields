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

        //x^2:
        math::array_operations<col_vector1,col_vector2,11> op;
        auto set_squares = [](Float& val1, const Float& val2)->void{ val1 = val2 * val2; };
        op.bmap(set_squares, quad_matrix_.column(1), points.column(1));
        op.bmap(set_squares, quad_matrix_.column(2), points.column(2));
        op.bmap(set_squares, quad_matrix_.column(3), points.column(3));
    }
};

}

#endif // QUAD_3D_FUN_H
