precision mediump float;
varying vec3 position;
uniform mat4 mvpMatrix;
uniform vec3 camPosition;

void main() {
    float d = distance(position,camPosition);
}