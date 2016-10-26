attribute vec4 vertex_position;
uniform mat4 mvp_matrix;
varying vec4 f_color;

void main(void)
{
    f_color = vec4(0.0, 0.0, 0.0, 1.0);
    gl_Position = mvp_matrix * vertex_position;
}
