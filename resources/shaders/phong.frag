#version 330 core

// Task 5: declare "in" variables for the world-space position and normal,
//         received post-interpolation from the vertex shader
in vec4 worldPos;
in vec3 worldNorm;
in vec3 worldTangent;

// Task 10: declare an out vec4 for your output color
out vec4 fragColor;

uniform vec4 camPos;

struct Material {
    vec4 cAmbient;  // Ambient term
    vec4 cDiffuse;  // Diffuse term
    vec4 cSpecular; // Specular term
    float shininess;      // Specular exponent
    float blend;
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

uniform bool isHighRes;

uniform bool useTexMap;
uniform sampler2D texMaps[8];

uniform bool useBumpMap;
uniform sampler2D bumpMaps[8];

vec4 getTexColor(float grass_cutoff, float rock_cutoff) {
    vec4 tex0, tex1;
    if (!isHighRes) {
        tex0 = texture(texMaps[0], vec2(worldPos[0], worldPos[2]) / 1.f);
        tex1 = texture(texMaps[1], vec2(worldPos[0], worldPos[2]) / 8.f);
    } else {
        tex0 = texture(texMaps[2], vec2(worldPos[0], worldPos[2]) / 2.f);
        tex1 = texture(texMaps[3], vec2(worldPos[0], worldPos[2]) / 1.f);
    }

    if (worldPos[1] < grass_cutoff) return tex1;
    if (worldPos[1] > rock_cutoff) return tex0;
    float a = (worldPos[1] - grass_cutoff) / (rock_cutoff - grass_cutoff);
    return a * tex0 + (1.f - a) * tex1;
}

vec4 getBumpColor(float grass_cutoff, float rock_cutoff) {
    vec4 tex0, tex1;
    if (!isHighRes) {
        tex0 = texture(bumpMaps[0], vec2(worldPos[0], worldPos[2]) / 8.f);
        tex1 = texture(bumpMaps[1], vec2(worldPos[0], worldPos[2]) / 8.f);
    } else {
        tex0 = texture(bumpMaps[2], vec2(worldPos[0], worldPos[2]) / 2.f);
        tex1 = texture(bumpMaps[3], vec2(worldPos[0], worldPos[2]) / 1.f);
    }


    if (worldPos[1] < grass_cutoff) return tex1;
    if (worldPos[1] > rock_cutoff) return tex0;
    float a = (worldPos[1] - grass_cutoff) / (rock_cutoff - grass_cutoff);
    return a * tex0 + (1.f - a) * tex1;
}

void main() {

    float grass_cutoff = 3.f; // everything below is grass
                              // everything between is blended
    float rock_cutoff =  6.f; // everything above is rock

    // Remember that you need to renormalize vectors here if you want them to be normalized
    vec3 normal = normalize(worldNorm);
    if (useBumpMap) {
        vec3 N = normalize(worldNorm);
        vec3 T = normalize(worldTangent);
        T = normalize(T - dot(T, N) * N);
        vec3 B = cross(T, N);
        vec3 BumpMapNormal = getBumpColor(grass_cutoff, rock_cutoff).xyz;
        BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.f);
        mat3 TBN = mat3(T, B, N);
        normal = normalize(TBN * BumpMapNormal);
    }

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
        if (useTexMap) {
            diff *= (1.f - material.blend);
            diff += material.blend * getTexColor(grass_cutoff, rock_cutoff);
        }
        diff *= max(0.f, dot(normal, light_vec));

        // add the specular term
//        vec3 reflected = reflect(-light_vec, normal);
//        float RdE = max(0.f, dot(reflected, normalize(vec3(camPos - worldPos))));
//        vec4 spec = Ks * material.cSpecular * ((RdE > 0) ? pow(RdE, material.shininess) : 0.f);
        vec4 spec = vec4(0.f);

        fragColor += atten * falloff * light.color * (diff + spec);
    }

}
