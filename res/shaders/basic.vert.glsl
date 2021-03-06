#version 130

// layout(location = 0) in vec3 vertexPosition_modelspace;

in vec3 vertex;

uniform mat4 MVP;

void main() {
    // gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
    gl_Position = MVP * vec4(vertex, 1);
}
