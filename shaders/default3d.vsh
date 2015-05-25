attribute highp vec3 a_vert;
attribute highp vec3 a_norm;
attribute highp vec3 a_tang;
attribute mediump vec2 a_uv;
attribute mediump vec4 a_bone;
attribute mediump vec4 a_rgba;
attribute mediump vec4 a_wght;

uniform highp mat4 u_camera;
uniform highp mat4 u_projection;
uniform highp vec4 u_light;
uniform highp vec3 u_offset;

uniform mat4 u_bone[32];
uniform vec4 u_bonepos[32];

varying mediump vec4 v_color;
varying mediump vec4 v_light;
varying vec3 v_norm;
varying vec3 v_uv;
varying vec3 v_normlight;
varying vec3 v_viewvec;
varying vec3 v_pos;

void main(void)
{
	highp vec4 vw;
	highp vec3 n;
	highp vec3 t;
	ivec4 idx = ivec4(a_bone);

	mat3 bone[4];

	bone[0] = mat3(u_bone[idx.x]);
	bone[1] = mat3(u_bone[idx.y]);
	bone[2] = mat3(u_bone[idx.z]);
	bone[3] = mat3(u_bone[idx.w]);
	
	n =  (bone[0] * a_norm) * a_wght.x;
	n += (bone[1] * a_norm) * a_wght.y;
	n += (bone[2] * a_norm) * a_wght.z;
	n += (bone[3] * a_norm) * a_wght.w;

	t =  (bone[0] * a_tang) * a_wght.x;
	t += (bone[1] * a_tang) * a_wght.y;
	t += (bone[2] * a_tang) * a_wght.z;
	t += (bone[3] * a_tang) * a_wght.w;

	vw =  (u_bone[idx.x] * vec4(a_vert - u_bonepos[idx.x].xyz, 1.0)) * a_wght.x;
	vw += (u_bone[idx.y] * vec4(a_vert - u_bonepos[idx.y].xyz, 1.0)) * a_wght.y;
	vw += (u_bone[idx.z] * vec4(a_vert - u_bonepos[idx.z].xyz, 1.0)) * a_wght.z;
	vw += (u_bone[idx.w] * vec4(a_vert - u_bonepos[idx.w].xyz, 1.0)) * a_wght.w;

	mat3 m3camera = mat3(u_camera);
	vec3 pos = normalize(m3camera * vec3(vw));
	vw = u_camera * vw;
	v_pos = vw.xyz / vw.w;
	vw.x += u_offset.x;
	vw.y += u_offset.y;
	vw.z += u_offset.z;
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
	v_viewvec.x = dot(t, pos);
	v_viewvec.y = dot(b, pos);
	v_viewvec.z = dot(n, pos);
	v_viewvec = normalize(v_viewvec);

	v_color = a_rgba;
	v_norm = n;
	v_uv = vec3(a_uv.x/vw.w, a_uv.y/vw.w, 1.0/vw.w);
	if(v_pos.z <= 0.0) {
		gl_Position = vw / vw.w;
	}
}
