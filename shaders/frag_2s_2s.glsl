#version 330 core
out vec4 FragColor;

in vec2 fragCoordNDC;

uniform float uTime;
uniform vec2 uMovingAtomPos; // moving atom

void main() {
    vec2 fixed_atom_pos = vec2(0.0, 0.0);  // fixed atom at center (in NDC)
    vec2 moving_atom_pos = 2.0 * uMovingAtomPos - 1.0;  // moving atom

    vec2 dist_to_fixed = fragCoordNDC - fixed_atom_pos;
    vec2 dist_to_moving = fragCoordNDC - moving_atom_pos;

    float r_fixed = length(dist_to_fixed);
    float r_moving = length(dist_to_moving);

    const float pi = 3.14159265359;

    // Orbital radii for 2s orbitals
    float orbital_radius_fixed = 0.25;
    float orbital_radius_moving = 0.25;

    // Normalization factors for 2s orbitals
    float normalization_fixed = 1.0 / sqrt(pi * pow(orbital_radius_fixed, 3.0));
    float normalization_moving = 1.0 / sqrt(pi * pow(orbital_radius_moving, 3.0));

    // ψ for fixed atom (2s orbital with radial node)
    float base_psi_fixed = normalization_fixed * (1.0 - r_fixed / orbital_radius_fixed) * exp(-r_fixed / orbital_radius_fixed);
    float phase_fixed = 0.0; // fixed atom with no phase rotation
    vec2 psi_fixed = base_psi_fixed * vec2(cos(phase_fixed), sin(phase_fixed));

    // ψ for moving atom (2s orbital with radial node)
    float base_psi_moving = normalization_moving * (1.0 - r_moving / orbital_radius_moving) * exp(-r_moving / orbital_radius_moving);
    float phase_moving = uTime; // moving atom with rotating phase
    vec2 psi_moving = base_psi_moving * vec2(cos(phase_moving), sin(phase_moving));

    // Total ψ: LCAO (linear combination of atomic orbitals)
    vec2 psi_total = psi_fixed + psi_moving;

    // Probability density (magnitude squared)
    float density = dot(psi_total, psi_total); // |ψ|² = ψ ψ*

    float intensity = density * 5.0;
    intensity = clamp(intensity, 0.0, 1.0);

    float real_component = psi_total.x;
    float imag_component = psi_total.y;

    vec3 color = vec3(
        0.5 + 0.5 * real_component / (base_psi_fixed + base_psi_moving + 0.0001), // Red for re
        0.0,                                                                      // Green
        0.5 + 0.5 * imag_component / (base_psi_fixed + base_psi_moving + 0.0001)  // Blue for img
    ) * intensity;

    FragColor = vec4(color, 1.0);
}
