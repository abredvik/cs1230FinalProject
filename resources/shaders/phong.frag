#version 330 core

// Task 5: declare "in" variables for the world-space position and normal,
//         received post-interpolation from the vertex shader
in vec4 worldPos;
in vec3 worldNorm;

// Task 10: declare an out vec4 for your output color
out vec4 fragColor;

uniform vec4 camPos;

struct Material {
    vec4 cAmbient;  // Ambient term
    vec4 cDiffuse;  // Diffuse term
    vec4 cSpecular; // Specular term
    float shininess;      // Specular exponent
};

uniform Material material;

struct SceneLightData {
    int type;

    vec4 color;
    vec3 function; // Attenuation function

    vec4 pos; // Position with CTM applied (Not applicable to directional lights)
    vec4 dir; // Direction with CTM applied (Not applicable to point lights)

    float penumbra; // Only applicable to spot lights, in RADIANS
    float angle;    // Only applicable to spot lights, in RADIANS
};

uniform int numLights;
uniform SceneLightData[8] lights;

uniform float Ka;
uniform float Kd;
uniform float Ks;

void main() {
    // Remember that you need to renormalize vectors here if you want them to be normalized
    vec3 normal = normalize(worldNorm);

    // Output illumination (we can ignore opacity)
    fragColor = vec4(0.0);

    // add the ambient term
    fragColor += Ka * material.cAmbient;

    for (int i = 0; i < numLights; ++i) {
        SceneLightData light = lights[i];

        // get direction vector to the light source
        vec4 dirToLight = (light.type == 1) ? -light.dir : light.pos - worldPos;

        // deal with different light types
        float falloff = 1.f, atten = 1.f;
        vec3 light_vec = vec3(dirToLight);
        if (light.type != 1) {
            // either spot light or point light
            if (light.type == 2) {
                // compute falloff
                float theta = acos(dot(normalize(light.dir), normalize(-dirToLight)));
                if (theta > light.angle) continue;
                float theta_inner = light.angle - light.penumbra;
                if (theta > theta_inner) {
                    float ratio = (theta - theta_inner) / (light.angle - theta_inner);
                    falloff = 1.f + ratio * ratio * ((2.f * ratio) - 3.f);
                }
            }

            // compute attenuation factor
            float dist = length(light_vec);
            vec3 distVec = vec3(1.f, dist, dist * dist);
            atten = min(1.f , 1.f / dot(distVec, light.function));
        }

        // add the diffuse term
        light_vec = normalize(light_vec);
        vec4 diff = Kd * material.cDiffuse;
        diff *= max(0.f, dot(normal, light_vec));

        // add the specular term
        vec3 reflected = reflect(-light_vec, normal);
        float RdE = max(0.f, dot(reflected, normalize(vec3(camPos - worldPos))));
        vec4 spec = Ks * material.cSpecular * ((RdE > 0) ? pow(RdE, material.shininess) : 0.f);

        fragColor += atten * falloff * light.color * (diff + spec);
    }

}
