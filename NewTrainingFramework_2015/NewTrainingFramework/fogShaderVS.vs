varying vec3 position;
uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;
uniform vec3 camPosition;

void main() {
    position = gl_Position * modelMatrix;
}