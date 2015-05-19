precision mediump float;
uniform vec4 u_rgba;
uniform sampler2D u_tex;

varying vec3 v_uv;


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
	vec4 color = texture2D(u_tex, uv);
	color = color * u_rgba;
	gl_FragColor = filters(color);
}
