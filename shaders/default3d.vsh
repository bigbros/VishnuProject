attribute highp vec3 a_vert;
attribute highp vec3 a_norm;
attribute highp vec3 a_tang;
attribute mediump vec2 a_uv;
attribute mediump vec4 a_bone;
attribute lowp vec4 a_rgba;
attribute lowp vec4 a_wght;

uniform highp mat4 u_camera;
uniform highp mat4 u_projection;
uniform highp vec4 u_light;
uniform highp vec4 u_offset;

#define BONES 32
uniform mat4 u_matrix;
uniform vec4 u_bone[BONES];
uniform vec4 u_boneorg[BONES];
uniform vec3 u_bonepos[BONES];


varying mediump vec4 v_color;
varying mediump vec4 v_light;
varying vec3 v_norm;
varying vec3 v_uv;
varying vec3 v_normlight;
varying vec3 v_viewvec;
varying vec3 v_view;
varying vec3 v_pos;


vec4 mulQuat(vec4 r, vec4 q) {
	return vec4(
		r.w*q.x + r.x*q.w + r.y*q.z - r.z*q.y,
		r.w*q.y - r.x*q.z + r.y*q.w + r.z*q.x,
		r.w*q.z + r.x*q.y - r.y*q.x + r.z*q.w,
		r.w*q.w - r.x*q.x - r.y*q.y - r.z*q.z);
}

vec3 rotQuat(vec4 q, vec3 v)
{
	vec4 vv = vec4(v, 0.0);
	vec4 r = vec4(-vec3(q), q.w);
	vec4 t = mulQuat(r, vv);
	return vec3(mulQuat(t, q));
}

void main(void)
{
	highp vec3 v;
	highp vec3 n;
	highp vec3 t;
	ivec4 idx = ivec4(a_bone);

	v  = (rotQuat(u_bone[idx.x], a_vert - vec3(u_boneorg[idx.x])) + u_bonepos[idx.x]) * a_wght.x;
/*
	v += (rotQuat(u_bone[idx.y], a_vert - vec3(u_boneorg[idx.y])) + u_bonepos[idx.y]) * a_wght.y;
	v += (rotQuat(u_bone[idx.z], a_vert - vec3(u_boneorg[idx.z])) + u_bonepos[idx.z]) * a_wght.z;
	v += (rotQuat(u_bone[idx.w], a_vert - vec3(u_boneorg[idx.w])) + u_bonepos[idx.w]) * a_wght.w;
*/

	n  = rotQuat(u_bone[idx.x], a_norm) * a_wght.x;
/*
	n += rotQuat(u_bone[idx.y], a_norm) * a_wght.y;
	n += rotQuat(u_bone[idx.z], a_norm) * a_wght.z;
	n += rotQuat(u_bone[idx.w], a_norm) * a_wght.w;
*/

	t  = rotQuat(u_bone[idx.x], a_tang) * a_wght.x;
/*
	t += rotQuat(u_bone[idx.y], a_tang) * a_wght.y;
	t += rotQuat(u_bone[idx.z], a_tang) * a_wght.z;
	t += rotQuat(u_bone[idx.w], a_tang) * a_wght.w;
*/

	highp vec4 vw = u_matrix * vec4(v, 1.0);
	mat3 m = mat3(u_matrix);
	n = m * normalize(n);
	t = m * normalize(t);

	mat3 m3camera = mat3(u_camera);
	vw = u_camera * vw;

	vw.x += u_offset.x * vw.w;
	vw.y += u_offset.y * vw.w;
	vw.z += u_offset.z * vw.w;
	
	v_pos = vw.xyz / vw.w;
	vw = u_projection * vw;

	n = normalize(n);
	t = normalize(t);
	vec3 b = normalize(cross(n, t));
	v_light = u_light;
	vec3 lgt = vec3(u_light);
	v_normlight.x = dot(t, lgt);
	v_normlight.y = dot(b, lgt);
	v_normlight.z = dot(n, lgt);
	v_normlight = normalize(v_normlight);

	n = m3camera * n;
	t = m3camera * t;
	b = cross(n, t);
	vec3 pos = -normalize(vec3(v_pos));
	v_viewvec.x = dot(t, pos);
	v_viewvec.y = dot(b, pos);
	v_viewvec.z = dot(n, pos);
	v_viewvec = normalize(v_viewvec);
	v_view = pos;
	v_color = a_rgba;
	v_norm = n;
	v_uv = vec3(a_uv.x/vw.w, a_uv.y/vw.w, 1.0/vw.w);
	if(v_pos.z <= 0.0) {
		gl_Position = vw / vw.w;
	}
}
