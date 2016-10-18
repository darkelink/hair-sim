#version 130


in float height;
in vec2 pos;

uniform mat4 MVP;
uniform float direction;
uniform float time;
uniform float strength;
uniform float frequency;
uniform int iterations;


// noise by Morgan McGuire @morgan3d http://graphicscodex.com
// Licenced and reused under the BSD licence

float hash(float n) { return fract(sin(n) * 1e4); }
float hash(vec2 p) { return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); }

float noise(vec3 x) {
    const vec3 step = vec3(110, 241, 171);

    vec3 i = floor(x);
    vec3 f = fract(x);

    float n = dot(i, step);

    vec3 u = f * f * (3.0 - 2.0 * f);
    return mix(mix(mix( hash(n + dot(step, vec3(0, 0, 0))), hash(n + dot(step, vec3(1, 0, 0))), u.x),
                   mix( hash(n + dot(step, vec3(0, 1, 0))), hash(n + dot(step, vec3(1, 1, 0))), u.x), u.y),
               mix(mix( hash(n + dot(step, vec3(0, 0, 1))), hash(n + dot(step, vec3(1, 0, 1))), u.x),
                   mix( hash(n + dot(step, vec3(0, 1, 1))), hash(n + dot(step, vec3(1, 1, 1))), u.x), u.y), u.z);
}



float fract_brownian(vec3 pos) {
    float freq = frequency;
    float total = 0.0;
    float amp = 0.5;
    for (int i = 0; i < iterations; ++i) {
        total += amp * noise(pos * freq);
        freq *= 2;
        amp /= 2;
    }
    return total;
}


void main() {
    float cd = cos(direction);
    float sd = sin(direction);

    float displacement = height * strength * fract_brownian(vec3(pos.x-cd*time, height, pos.y-sd*time));

    gl_Position = MVP * vec4(
            pos.x + displacement*cd,
            sqrt(height*height - displacement*displacement),
            pos.y + displacement*sd,
            1);
}
