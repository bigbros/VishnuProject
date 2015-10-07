precision mediump float;
uniform vec4 u_rgba;
uniform vec4 u_ambient;

uniform vec4 u_modelcol;
uniform float u_shininess;
//uniform int u_switch;
uniform sampler2D u_tex;
uniform sampler2D u_normmap;
uniform sampler2D u_specularmap;

uniform bool u_f_normalmap;
uniform bool u_f_texmap;
uniform bool u_f_specularmap;

varying mediump vec4 v_color;
varying mediump vec4 v_light;
varying vec3 v_norm;
varying vec3 v_uv;
varying vec3 v_normlight;
varying vec3 v_viewvec;
varying vec3 v_view;
varying vec3 v_pos;

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
	if(v_pos.z >= 0.0) {
		discard;
	} else {
		vec2 uv = vec2(v_uv / v_uv.z);
		vec3 norm;
		vec3 light;
		vec3 view;
		if(u_f_normalmap) {
			norm = vec3(texture2D(u_normmap, uv)) * 2.0 - 1.0;
			light = normalize(v_normlight);
			view = v_viewvec;
		} else {
			norm = v_norm;
			light = normalize(vec3(v_light));
			view = v_view;
		}
		//if(dot(norm, view) < 0.0) discard;
		
		float diffuse = max(dot(light, norm), 0.0);
		//diffuse=0.0;

		// specular
		//view = normalize(view);
		float specular = 0.0;
		vec3 refvec = reflect(-light, norm);
		specular = pow(max(dot(view, refvec), 0.0), 1.0/u_shininess);
		if(dot(norm, light) < 0.0) specular = 0.0;
		//specular = 0.0;
		vec4 spcol = u_rgba * specular;

		vec4 color;
		if(u_f_texmap) {
			color = texture2D(u_tex, uv);
		} else {
			color = vec4(1.0, 1.0, 1.0, 1.0);
		}
		color = color * v_color * u_modelcol * u_rgba * diffuse + u_ambient * (1.0 - diffuse) + spcol;
		gl_FragColor = filters(color);
	}
}
