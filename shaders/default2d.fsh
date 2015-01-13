precision mediump float;

uniform vec4 u_bright;

uniform sampler2D u_tex;

varying vec2 v_uv;
varying vec4 v_rgba;

void main(void)
{
	gl_FragColor = texture2D(u_tex, v_uv) * v_rgba; // * u_bright;
}
