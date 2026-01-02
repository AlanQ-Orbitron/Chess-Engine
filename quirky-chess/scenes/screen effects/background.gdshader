shader_type canvas_item;

uniform vec3 primary;
uniform vec3 secondary;

vec3 swap(vec3 current, vec3 color, vec3 target) {
	float isTarget = step(length(current - target), 0.01);
	return mix(current, color, isTarget);
}

void fragment() {
	vec2 uv = UV;
	uv /= 2.0;
	uv.x += sin(TIME) * 0.02;
	uv.y += TIME * -0.1;
	vec3 color = texture(TEXTURE, uv).rgb;
	
	color = swap(color, primary, vec3(1, 1, 1));
	color = swap(color, secondary, vec3(0, 0, 0));
	
	COLOR = vec4(color, 1.0);
}
