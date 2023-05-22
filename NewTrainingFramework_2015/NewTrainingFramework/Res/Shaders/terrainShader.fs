precision mediump float;
varying vec2 v_uv;
varying vec2 v_uv_bmap;
uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;
uniform vec3 u_fogColor;
uniform vec3 u_camPosition;
varying vec3 position;
uniform float u_smallRadius;
uniform float u_bigRadius;


void main()
{
    vec4 blend_texture = texture2D(u_texture_0,v_uv_bmap);
    vec4 texture1 = texture2D(u_texture_1,v_uv);
    vec4 texture2 = texture2D(u_texture_2,v_uv);
    vec4 texture3 = texture2D(u_texture_3,v_uv);
    vec4 color = blend_texture.r * texture1 + blend_texture.g * texture2 + blend_texture.b * texture3;
    float d = distance(position,u_camPosition);
    float alpha = clamp( (d-u_smallRadius)/(u_bigRadius-u_smallRadius) , 0.0, 1.0);
    color = clamp(vec4(alpha * u_fogColor,0.0) + (1.0 - alpha) * color, 0.0, 1.0);
    gl_FragColor = color;
}