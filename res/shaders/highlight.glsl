#shader vertex
#version 410 core
layout(location = 0) in vec3 aPos;
uniform mat4 u_MVP;
void main() {
    gl_Position = u_MVP * vec4(aPos, 1.0);
}

#shader fragment
#version 410 core
out vec4 FragColor;
uniform vec4 u_Color;
void main() {
    FragColor = u_Color;
}
