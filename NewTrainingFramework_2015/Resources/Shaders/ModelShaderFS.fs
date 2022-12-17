precision mediump float;
varying vec2 v_uv;

void main()
{
    gl_FragColor = vec4(v_uv,1.0,1.0);
}