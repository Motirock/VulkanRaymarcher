#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in flat vec3 normal;
layout(location = 2) in vec3 fragColor;
layout(location = 3) in float distanceFromCamera;

layout(location = 0) out vec4 outColor;

vec3 lightDirection = normalize(vec3(-1.0, -1.0, -1.0));
vec3 lightColor = vec3(1.0, 1.0, 1.0);
float ambient = 0.2f;

float maxDistanceAir = 5000.0;
float minDistanceAir = 100.0;
float distanceFadeAir() {
    return clamp((distanceFromCamera - minDistanceAir) / (maxDistanceAir - minDistanceAir), 0.0, 1.0);
}

vec4 skyColor = vec4(0.2f, 0.25f, 0.7f, 1.0f);

void main() {
    if (distanceFromCamera > maxDistanceAir) {
        outColor = skyColor;
        return;
    }

    vec4 sampledColor = texture(texSampler, fragTexCoord);
    if (sampledColor.w == 0.0f)
        discard;

    float diff = max(dot(normal, -lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * sampledColor.rgb * fragColor;
    
    outColor = vec4(result, sampledColor.a)*(1.0f-distanceFadeAir())+skyColor*distanceFadeAir();
}
