precision mediump float;
uniform vec4 u_rgba;
uniform vec4 u_ambient;

uniform vec4 u_modelcol;
uniform float u_shininess;
uniform int u_switch;
uniform sampler2D u_tex;
uniform sampler2D u_normmap;

varying mediump vec4 v_color;
varying mediump vec4 v_light;
varying vec3 v_norm;
varying vec3 v_uv;
varying vec3 v_normlight;
varying vec3 v_viewvec;

#define TEXTURE  0x00000001
#define NORMAL   0x00000002
#define DIFFUSE  0x00000004
#define SPECULAR 0x00000008

vec4 grayScale(vec4 color)
{
	color = pow(color, vec4(2.2, 2.2, 2.2, 1.0)) * vec4(0.22, 0.71, 0.07
	, 1.0);
	float lev = color.r + color.g + color.b;
	return pow(vec4(lev, lev, lev, color.a), vec4(1.0 / 2.2, 1.0/2.2, 1.0/2.2, 1.0));
}


vec4 filters(vec4 color)
{
	return color; // grayScale(color);
}

void main(void)
{
	vec2 uv = vec2(v_uv / v_uv.z);
	vec3 norm = vec3(texture2D(u_normmap, uv)) * 2.0 - 1.0;
	//vec3 light = normalize(vec3(v_light));
	float diffuse = max(dot(v_normlight, norm), 0.0); // max(dot(light, v_norm), 0.0);

	vec3 view = normalize(-v_viewvec);
	vec3 refvec = reflect(norm, v_normlight);
	float specular = pow(max(dot(view, refvec), 0.0), u_shininess);
	vec4 spcol = u_rgba * specular;
	
	vec4 color = texture2D(u_tex, uv); // : vec4(1.0, 1.0, 1.0, 1.0);
	color = color * v_color * u_modelcol * u_rgba * diffuse + u_ambient * (1.0 - diffuse) + spcol;
	gl_FragColor = filters(color);
}
