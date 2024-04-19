#version 450

layout(binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {

    //vec2 coord = gl_PointCoord - vec2(0.5);
    outColor = vec4(fragColor.xyz, 1.0f);
    outColor = texture(texSampler, gl_PointCoord);
}