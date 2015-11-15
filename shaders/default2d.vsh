attribute highp vec2 a_vert;
attribute mediump vec2 a_params;

uniform highp mat4 u_projection;

#define DRAW_NUM 16
uniform mat3 u_mat[DRAW_NUM];
uniform vec4 u_uv[DRAW_NUM];
uniform vec4 u_rgba[DRAW_NUM];

varying vec2 v_uv;
varying vec4 v_rgba;

void main(void)
{
	int idx = int(a_params.x);

	vec2 lt = vec2(u_uv[idx].x, u_uv[idx].y);
	vec2 rb = vec2(u_uv[idx].z, u_uv[idx].w);
	
	v_uv = (rb - lt) * a_vert + lt;
	v_rgba = u_rgba[idx];
	vec3 v = u_mat[idx] * vec3(a_vert, 1.0);
	gl_Position = u_projection * vec4(v, 1.0);
}
