shader_type canvas_item;
render_mode unshaded, skip_vertex_transform;

uniform sampler2D screen_texture: hint_screen_texture;
uniform vec2 offset;
uniform float chromatic_abbreation_strength;
uniform vec2 screen_size;

const float curvature = 7.0;

void fragment() {
	vec2 centered_uv = (UV * 2.0 - 1.0) + offset * vec2(screen_size.y / screen_size.x, 1.0);
	
	vec2 uv_offset = centered_uv.yx / curvature;
	vec2 warped_uv = centered_uv + centered_uv * uv_offset * uv_offset;
	vec3 cutoff = vec3(step(abs(warped_uv.x), 1.0) * step(abs(warped_uv.y), 1.0));
	vec3 vintage = vec3(length(pow(abs(centered_uv), vec2(16.0)) / 4.0));
	vec2 recentered_uv = (warped_uv + 1.0) / 2.0;
	
	float r = texture(screen_texture, recentered_uv + offset / chromatic_abbreation_strength).r;
	float g = texture(screen_texture, recentered_uv).g;
	float b = texture(screen_texture, recentered_uv - offset / chromatic_abbreation_strength).b;
	vec3 chromatic_abbreation = vec3(r, g, b) * cutoff;
	
	chromatic_abbreation -= vintage;
	COLOR = vec4(chromatic_abbreation, 1.0);
}