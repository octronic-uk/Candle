#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform mat4 mv_matrix;

attribute vec4 a_position;
attribute vec4 a_color;
attribute vec4 a_start;

varying vec4 v_color;
varying vec2 v_position;
varying vec2 v_start;
varying vec2 v_texture;

void main()
{
    gl_PointSize = a_start.z;
    gl_Position = mvp_matrix * a_position;
    v_color.wxyz = a_color.wxyz;
}
