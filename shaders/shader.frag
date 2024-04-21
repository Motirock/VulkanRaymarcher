#version 450

layout (binding = 0) uniform sampler2D samplerColor;

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(samplerColor, (gl_PointCoord+vec2(1, 1))/2.0f);
}