#version 410 core

in vec3 pos;

uniform float system_time;
uniform vec2 resolution;
uniform vec3 mouse;

out float v_system_time;
out vec2 v_resolution;
out vec3 v_mouse;

void main(){
    gl_Position = vec4(pos,1.0);
    v_system_time = system_time;
    v_resolution = resolution;
    v_mouse = mouse;
}