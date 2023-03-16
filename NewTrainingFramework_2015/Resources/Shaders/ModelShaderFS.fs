precision mediump float;
varying vec2 v_uv;
uniform sampler2D u_texture;
uniform vec3 u_fogColor;
uniform vec3 u_camPosition;
varying vec3 position;
uniform float u_smallRadius;
uniform float u_bigRadius;

void main()
{
    vec4 color = texture2D(u_texture,v_uv);
    if(color.a < 0.2) 
        discard;

    float d = distance(position,u_camPosition);
    float alpha = clamp( (d-u_smallRadius)/(u_bigRadius-u_smallRadius) , 0.0, 1.0);
    color = clamp(vec4(alpha * u_fogColor,0.0) + (1.0 - alpha) * color, 0.0, 1.0);
    gl_FragColor = color;
}