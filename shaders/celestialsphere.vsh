attribute highp vec3 a_vert;
attribute mediump vec2 a_uv;

uniform highp mat4 u_camera;
uniform highp mat4 u_projection;

varying vec3 v_uv;

void main(void)
{
	// Celestial Sphere is rotation only.
	mat3 camera = mat3(u_camera);
	vec3 v = camera * a_vert;
	vec4 vw = vec4(v, 1.0);
	vw = u_projection * vw;

	v_uv = vec3(a_uv.x/vw.w, a_uv.y/vw.w, 1.0/vw.w);
	gl_Position = vw / vw.w;;
}
