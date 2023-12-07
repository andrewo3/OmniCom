#version 330 core
in vec2 FragTexCoord;
out vec4 FragColor;

uniform sampler2D textureSampler;
uniform float iTime;

/*void main() {
    FragColor = texture(textureSampler, FragTexCoord);
}*/

//SHADER CODE IS NOT MINE: https://www.shadertoy.com/view/dtGyzR
// VHS Scanline Effect
// by McStuffings 2023
// License: MIT

void main() {
    vec2 uv = FragTexCoord;

    // Scanline settings
    float blendFactor = 0.01; // Use lower values for subtle scanline effect.
    float scanlineHeight = 4.0;
    float scanlineIntensity = 1;
    float scrollSpeed = 5.0; // Use +/- values for up/down movement.
    vec3 color = vec3(1.0, 1.0, 1.0);
    
    // Grain settings
    float grainIntensity = 10;
    vec2 grainSeed = vec2(12.9898, 78.233) + iTime * 0.1; // Varying the seed over time
    
    // Glitch settings
    // Use higher values in this section for extreme effect.
    float glitchProbability = 0.000001;
    float glitchIntensityX = 0.002; // Intensity of the horizontal jitter
    float glitchIntensityY = .0002; // Intensity of the vertical jitter
    
    // Vertical Band settings
    float bandSpeed = 1.0;
    float bandHeight = 0.01;
    float bandIntensity = 0.03;
    float bandChoppiness = 0.02;
    float staticAmount = 0.02;
    float warpFactor = .05;
    float chromaAmount = .3;
    
    // Moving VHS effect
    float scanline = sin((uv.y - iTime * scrollSpeed) * (1.0/scanlineHeight));
    vec3 vhsColor = color * scanline * scanlineIntensity;

    // Grain effect
    float grain = fract(sin(dot(FragTexCoord * uv, grainSeed)) * 43758.5453);
    vhsColor += grain * grainIntensity;

    // Glitch Effect
    if (fract(iTime) < glitchProbability) {
        float glitchOffsetX = (fract(sin(iTime * 12.9898) * 43758.5453) - 0.5) * glitchIntensityX;
        float glitchOffsetY = (fract(cos(iTime * 78.233) * 43758.5453) - 0.5) * glitchIntensityY;
        uv += vec2(glitchOffsetX, glitchOffsetY);
    }

    // VHS Band
    float bandPos = fract(iTime * bandSpeed);
    float bandNoise = fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);

    if (abs(uv.y - bandPos) < bandHeight) {
        // Add static with choppiness
        float randomStatic = bandNoise * bandChoppiness;
        vhsColor += vec3(randomStatic) * staticAmount;

        // Add warp effect with choppiness
        uv.x += sin(uv.y * 10.0 + randomStatic) * warpFactor;

        // Chromatic aberration with choppiness
        vec3 chromaColor = vec3(
            texture(textureSampler, uv + vec2(chromaAmount * randomStatic, 0.0)).r,
            texture(textureSampler, uv).g,
            texture(textureSampler, uv - vec2(chromaAmount * randomStatic, 0.0)).b
        );

        // Mix chromatic aberration with reduced intensity
        float adjustedIntensity = bandIntensity * (1.0 - randomStatic);
        vhsColor = mix(vhsColor, chromaColor, adjustedIntensity);
    }

    // Original color before applying scanlines (but after the glitch, noise, and vertical band)
    // If you have an input texture, fetch the color from the texture here
    vec3 originalColor = vec3(texture(textureSampler, uv+vec2(0.0006,0)).r,texture(textureSampler, uv+vec2(-0.0005,0)).g,texture(textureSampler, uv).b);

    // Blend the original color with the VHS effect
    vec3 finalColor = mix(originalColor, vhsColor, blendFactor);

    FragColor = vec4(finalColor, 1.0);
}