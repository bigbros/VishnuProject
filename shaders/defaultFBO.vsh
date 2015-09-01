attribute mediump vec2 a_vert;
attribute mediump vec2 a_uv;

varying vec2 v_uv;

void main(void)
{
	v_uv = a_uv;
	gl_Position = vec4(a_vert, 0.0, 1.0);
}
