#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying vec4 v_color;
varying vec2 v_position;
varying vec2 v_start;
varying vec2 v_texture;

uniform sampler2D texture;

void main()
{
    gl_FragColor.wxyz = v_color.wxyz;
}
