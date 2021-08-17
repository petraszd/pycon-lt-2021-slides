shader_type canvas_item;

uniform vec4 inside_color : hint_color;
uniform vec4 outside_color : hint_color;

void fragment(){
    float len = clamp(length(UV.xy - vec2(0.5f, 0.5f)) - 0.25f, 0.0f, 1.0f);
    COLOR = outside_color * len + (1.0f - len) * inside_color;
}