attribute vec4 vertex_position;
uniform mat4 mvp_matrix;

void main(void)
{
    gl_Position = mvp_matrix * vertex_position;
}
