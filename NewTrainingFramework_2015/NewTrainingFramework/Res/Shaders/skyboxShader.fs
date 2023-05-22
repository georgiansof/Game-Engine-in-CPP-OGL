precision mediump float;
varying vec2 v_uv;
varying vec3 position;
uniform samplerCube u_cube_texture;

void main() {
    gl_FragColor = textureCube(u_cube_texture,position);
}