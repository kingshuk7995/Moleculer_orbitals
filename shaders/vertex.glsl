#version 330 core
layout(location = 0) in vec3 aPos;

out vec2 fragCoordNDC;

void main() {
    gl_Position = vec4(aPos, 1.0);
    fragCoordNDC = aPos.xy; // passing x,y of the fragment coordinates normalised
}
