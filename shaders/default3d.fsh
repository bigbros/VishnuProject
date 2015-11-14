precision mediump float;

uniform vec4 u_modelcol;
uniform vec4 u_rgba;

uniform vec4 u_ambient;
uniform vec4 u_diffuse;
uniform vec4 u_emissive;
uniform vec4 u_specular;

uniform float u_ambient_f;
uniform float u_diffuse_f;
uniform float u_emissive_f;
uniform float u_shininess;

uniform sampler2D u_maptexture;
uniform sampler2D u_mapnormal;
uniform sampler2D u_mapspecular;

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
		// ambient
		vec4 ambient = u_ambient * u_ambient_f;

		// diffuse
		vec2 uv = vec2(v_uv / v_uv.z);
		vec3 norm;
		vec3 light;
		vec3 view;
		if(u_f_normalmap) {
			norm = vec3(texture2D(u_mapnormal, uv)) * 2.0 - 1.0;
			light = normalize(v_normlight);
			view = v_viewvec;
		} else {
			norm = v_norm;
			light = normalize(vec3(v_light));
			view = v_view;
		}
		//if(dot(norm, view) < 0.0) discard;
		
		float diffuse = u_diffuse_f * max(dot(light, norm), 0.0);

		// specular
		float specular = 0.0;
		float shininess = u_shininess;
		vec4 spcol = u_rgba;
		if(u_f_specularmap) {
			vec4 sp_tex = texture2D(u_mapspecular, uv);
			shininess = shininess * sp_tex.a;
			spcol = spcol * sp_tex;
			spcol.a = 1.0;
		}
		if(shininess > 0.0) {
			vec3 refvec = reflect(-light, norm);
			specular = pow(max(dot(view, refvec), 0.0), shininess);
			if(dot(norm, light) < 0.0) specular = 0.0;
		}
		spcol = spcol * u_specular * specular;

		vec4 color;
		if(u_f_texmap) {
			color = texture2D(u_maptexture, uv);
		} else {
			color = vec4(1.0, 1.0, 1.0, 1.0);
		}
		color = color * v_color * u_modelcol * u_rgba * u_diffuse * diffuse + ambient * (1.0 - diffuse) + spcol + u_emissive * u_emissive_f;
		gl_FragColor = filters(color);
	}
}
