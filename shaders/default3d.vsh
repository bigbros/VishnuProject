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
uniform highp vec4 u_offset;

#define BONES 16
uniform mat4 u_bone[BONES];
uniform vec4 u_bonepos[BONES];

varying mediump vec4 v_color;
varying mediump vec4 v_light;
varying vec3 v_norm;
varying vec3 v_uv;
varying vec3 v_normlight;
varying vec3 v_viewvec;
varying vec3 v_view;
varying vec3 v_pos;

void main(void)
{
	highp vec4 vw;
	highp vec3 n;
	highp vec3 t;
	{
		ivec4 idx = ivec4(a_bone);
		mat4 bone4[4];
		vec4 bonepos[4];
		float wght[4];

		bonepos[0] = u_bonepos[idx.x];
		bonepos[1] = u_bonepos[idx.y];
		bonepos[2] = u_bonepos[idx.z];
		bonepos[3] = u_bonepos[idx.w];

		wght[0] = a_wght.x;
		wght[1] = a_wght.y;
		wght[2] = a_wght.z;
		wght[3] = a_wght.w;
	
		bone4[0] = u_bone[idx.x];
		bone4[1] = u_bone[idx.y];
		bone4[2] = u_bone[idx.z];
		bone4[3] = u_bone[idx.w];

		n = vec3(0.0);
		t = vec3(0.0);
		vw = vec4(0.0);
		for(int i = 0; i < 4; i++) {
			mat3 bone = mat3(bone4[i]);
			n += (bone * a_norm) * wght[i];
			t += (bone * a_tang) * wght[i];
			vw += (bone4[i] * vec4(a_vert - vec3(bonepos[i]), 1.0)) * wght[i];
		}
	}

	mat3 m3camera = mat3(u_camera);
	//vec3 pos = normalize(m3camera * vec3(vw));
	vw = u_camera * vw;
	vec3 pos = normalize(vec3(vw));

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

	v_viewvec.x = dot(t, -pos);
	v_viewvec.y = dot(b, -pos);
	v_viewvec.z = dot(n, -pos);
	v_viewvec = normalize(v_viewvec);
	v_view = -pos;
	v_color = a_rgba;
	v_norm = n;
	v_uv = vec3(a_uv.x/vw.w, a_uv.y/vw.w, 1.0/vw.w);
	if(v_pos.z <= 0.0) {
		gl_Position = vw / vw.w;
	}
}
