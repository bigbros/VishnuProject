attribute mediump vec2 a_vert;
attribute mediump vec3 a_uv;

uniform float u_vrate;
varying vec3 v_uv;

void main(void)
{
	v_uv = a_uv;
	gl_Position = vec4(a_vert.x, a_vert.y * u_vrate, 0.0, 1.0);
}
