#shader vertex

#version 410 core
layout(location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}

#shader fragment

#version 410 core
out vec4 FragColor;
uniform vec4 u_Color;
void main() {
    FragColor = u_Color;
}
