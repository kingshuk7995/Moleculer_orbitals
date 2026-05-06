#version 330 core
out vec4 FragColor;

in vec2 fragCoordNDC;

uniform float uTime;
uniform vec2 uMovingAtomPos;
uniform float uIntensityScale;
uniform float uPhaseDiff;
uniform float uBohrRadius;

uniform mat3 uRotation;

uniform int uN1;
uniform int uL1;
uniform int uM1;

uniform int uN2;
uniform int uL2;
uniform int uM2;

const float PI = 3.14159265359;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float factorial(int n) {
    if (n <= 1) return 1.0;
    float f = 1.0;
    for(int i = 2; i <= n; i++) f *= float(i);
    return f;
}

float double_factorial(int n) {
    if (n <= 0) return 1.0;
    float f = 1.0;
    for(int i = n; i > 0; i -= 2) f *= float(i);
    return f;
}

float laguerre(int n, int alpha, float x) {
    if (n == 0) return 1.0;
    if (n == 1) return float(1 + alpha) - x;
    
    float L0 = 1.0;
    float L1 = float(1 + alpha) - x;
    float L2 = 0.0;
    for (int k = 1; k < n; k++) {
        L2 = ((float(2 * k + 1 + alpha) - x) * L1 - float(k + alpha) * L0) / float(k + 1);
        L0 = L1;
        L1 = L2;
    }
    return L1;
}

float assoc_legendre_nocs(int l, int m, float x) {
    int abs_m = abs(m);
    if (abs_m > l) return 0.0;
    
    float p_mm = 1.0;
    if (abs_m > 0) {
        p_mm = double_factorial(2 * abs_m - 1) * pow(1.0 - x * x, float(abs_m) / 2.0);
    }
    
    if (l == abs_m) return p_mm;
    
    float p_mp1_m = x * float(2 * abs_m + 1) * p_mm;
    if (l == abs_m + 1) return p_mp1_m;
    
    float p_lm_minus_2 = p_mm;
    float p_lm_minus_1 = p_mp1_m;
    float p_l = 0.0;
    
    for (int i = abs_m + 2; i <= l; i++) {
        p_l = (float(2 * i - 1) * x * p_lm_minus_1 - float(i + abs_m - 1) * p_lm_minus_2) / float(i - abs_m);
        p_lm_minus_2 = p_lm_minus_1;
        p_lm_minus_1 = p_l;
    }
    return p_l;
}

vec2 evaluate_psi(int n, int l, int m, vec3 pos, float phase_offset) {
    float r = length(pos);
    if (r < 1e-6) return vec2(0.0);
    
    float theta = acos(clamp(pos.z / r, -1.0, 1.0));
    float phi = atan(pos.y, pos.x);
    
    float a0 = uBohrRadius;
    float rho = 2.0 * r / (float(n) * a0);
    
    // Radial part
    float norm_r = sqrt(pow(2.0 / (float(n) * a0), 3.0) * factorial(n - l - 1) / (2.0 * float(n) * factorial(n + l)));
    float R_nl = norm_r * exp(-r / (float(n) * a0)) * pow(rho, float(l)) * laguerre(n - l - 1, 2 * l + 1, rho);
    
    // Angular part (Spherical Harmonics)
    int abs_m = abs(m);
    float norm_y = sqrt(float(2 * l + 1) / (4.0 * PI) * factorial(l - abs_m) / factorial(l + abs_m));
    
    // Apply Condon-Shortley phase conventionally only for +m
    float phase_sign = (m > 0 && (m % 2 != 0)) ? -1.0 : 1.0;
    
    float P_lm = assoc_legendre_nocs(l, abs_m, cos(theta));
    float Y_lm_mag = phase_sign * norm_y * P_lm;
    
    // Complex phase
    float total_phase = float(m) * phi + phase_offset;
    
    return R_nl * Y_lm_mag * vec2(cos(total_phase), sin(total_phase));
}

void main() {
    // 3D coordinates based on 2D slice
    vec3 slice_pos = vec3(fragCoordNDC.x, fragCoordNDC.y, 0.0);
    
    // Rotate the 3D space so the viewer can slice at any angle
    vec3 world_pos = uRotation * slice_pos;
    
    vec3 fixed_atom_pos = vec3(0.0, 0.0, 0.0);
    vec3 moving_atom_pos = vec3(2.0 * uMovingAtomPos.x - 1.0, 2.0 * uMovingAtomPos.y - 1.0, 0.0);
    
    vec3 pos_rel_fixed = world_pos - fixed_atom_pos;
    vec3 pos_rel_moving = world_pos - moving_atom_pos;
    
    vec2 psi_1 = evaluate_psi(uN1, uL1, uM1, pos_rel_fixed, uTime);
    vec2 psi_2 = evaluate_psi(uN2, uL2, uM2, pos_rel_moving, uTime + uPhaseDiff);
    
    vec2 psi_total = psi_1 + psi_2;
    float density = dot(psi_total, psi_total); // |psi|^2
    
    float intensity = density * uIntensityScale;
    intensity = clamp(intensity, 0.0, 1.0);
    
    float phase = atan(psi_total.y, psi_total.x);
    float hue = (phase / (2.0 * PI)) + 0.5;
    
    vec3 color = hsv2rgb(vec3(hue, 1.0, intensity));
    
    FragColor = vec4(color, 1.0);
}
