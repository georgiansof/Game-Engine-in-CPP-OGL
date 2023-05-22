precision mediump float;
varying vec2 v_uv;
uniform sampler2D u_texture_0; /// displacement
uniform sampler2D u_texture_1; /// blend map
uniform sampler2D u_texture_2;
uniform vec3 u_fogColor;
uniform vec3 u_camPosition;
varying vec3 position;
uniform float u_smallRadius;
uniform float u_bigRadius;

uniform float u_dispMax;
uniform float u_time;


void main()
{
    //vec2 offset = (vec2(v_uv.x,v_uv.y + u_time) * 2.0 - 1.0) * u_dispMax;
    vec4 displacementTexture = texture2D(u_texture_0,vec2(v_uv.x,v_uv.y + u_time));
    vec2 disp = displacementTexture.rg;
    vec2 offset = ( disp * 2.0 - 1.0 ) * u_dispMax;
    vec2 v_uv_displaced = v_uv + offset;
    vec4 greyscaleMask = texture2D(u_texture_1,vec2(v_uv.x,1.0-v_uv.y));
    vec4 fireTexture = texture2D(u_texture_2,v_uv_displaced);

    fireTexture.a *= greyscaleMask.r; /// r = g = b
    vec4 color = fireTexture;

    float d = distance(position,u_camPosition);
    float alpha = clamp( (d-u_smallRadius)/(u_bigRadius-u_smallRadius) , 0.0, 1.0);
    color = clamp(vec4(alpha * u_fogColor,0.0) + (1.0 - alpha) * color, 0.0, 1.0);
    gl_FragColor = color;
}