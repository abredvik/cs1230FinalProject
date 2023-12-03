#version 330 core

// Task 16: Create a UV coordinate in variable
in vec2 uv;

// Task 8: Add a sampler2D uniform
uniform sampler2D tex;

// Task 29: Add a bool on whether or not to filter the texture
uniform bool perPixelFilter;
uniform bool kernelFilter;

uniform int width;
uniform int height;

out vec4 fragColor;

void main()
{
    fragColor = vec4(1);
    // Task 17: Set fragColor using the sampler2D at the UV coordinate
    fragColor = texture(tex, uv);

    if (kernelFilter) {
        fragColor = vec4(0);
        for (int i = -2; i <= 2; ++i) {
            for (int j = -2; j <= 2; ++j) {
                vec2 trans = uv + vec2(float(i) / float(width), float(j) / float(height));
                fragColor += texture(tex, trans) / 25.f;
            }
        }
    }

    // Task 33: Invert fragColor's r, g, and b color channels if your bool is true
    if (perPixelFilter) fragColor = 1 - fragColor;
}
