precision mediump float;
uniform sampler2D u_tex_r;
uniform sampler2D u_tex_l;
uniform float u_angle;
uniform float u_f;
uniform float u_w;

varying vec3 v_uv;

vec4 barrel(sampler2D tex, float w, float f, vec2 c, vec2 p)
{
//	vec2 uv = p * 2.0 - 1.0;
	vec2 uv = (p - c) / vec2(w, 0.5);
	float dist = length(uv);
	float l = u_f * tan(u_angle / 2.0 * dist);
	uv = uv * l / dist;
	uv = (uv + 1.0) / 2.0;
	if(uv.x < 0.0 || uv.y < 0.0 || uv.x > 1.0 || uv.y > 1.0) {
		return vec4(0.0, 0.0, 0.0, 0.0);
	}
	return texture2D(tex, uv);
}
 
void main(void)
{
	int t = int(v_uv.z);
	vec4 col;
	float uc = u_angle / 2.0;
	if(t == 0) {
		vec2 c = vec2(u_w, 0.5);
		col = barrel(u_tex_r, u_w, u_f, c, vec2(v_uv));
	} else {
		vec2 c = vec2(1.0-u_w, 0.5);
		col = barrel(u_tex_l, u_w, u_f, c, vec2(v_uv));
	}
	gl_FragColor = col;
}
