precision mediump float;
uniform sampler2D u_tex;

varying vec2 v_uv;


void main(void)
{
	gl_FragColor = texture2D(u_tex, v_uv);
}
