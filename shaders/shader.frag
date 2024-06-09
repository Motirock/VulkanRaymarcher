#version 450

//TODO different dsecrotor set for frag

layout (binding = 0) uniform sampler2D imageSampler;

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(imageSampler, (gl_PointCoord+vec2(1, 1))/2.0f);
}