uniform mat4 mvp_matrix;
attribute vec4 vertex_position;

void main(void)
{
    gl_Position = mvp_matrix * vertex_position;
}
