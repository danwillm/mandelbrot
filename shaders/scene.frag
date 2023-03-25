#version 410 core

precision highp float;

layout(location = 0) out vec4 frag_color;

in float v_system_time;
in vec2 v_resolution;
in vec3 v_mouse;

float iterations = 100;

vec2 GetCoordinatesFromScreen(vec2 fragCoord, vec2 vSystemResolution, vec4 coordinateRange){
    vec2 normalizedCoordinates = gl_FragCoord.xy/vSystemResolution;

    float horizontalSize = coordinateRange.y-coordinateRange.x;
    float verticalSize = coordinateRange.w-coordinateRange.z;

    normalizedCoordinates.x = normalizedCoordinates.x*horizontalSize+coordinateRange.x;
    normalizedCoordinates.y = normalizedCoordinates.y*verticalSize+coordinateRange.z;

    return normalizedCoordinates;
}

float DoMandelbrot(vec2 coord){
    vec2 z = vec2(0,0);
    for(int i = 0; i < iterations; i++) {

        //z^2 = (x + iy)^2 = x^2 + 2xiy + -y^2
        z = vec2(pow(z.x,2) - pow(z.y,2), 2*z.x*z.y) + coord;

        if(length(z) > 2) {
            return i/iterations;
        }
    }

    return iterations;
}


void main(){
    vec4 coords = vec4(-1.0,1.0,-1.0,1.0);
    vec4 translate = vec4(vec2(v_mouse.x), vec2(v_mouse.y));

    vec2 pixel_coordinates = GetCoordinatesFromScreen(gl_FragCoord.xy, v_resolution, (coords + translate) * v_mouse.z);
    float color = DoMandelbrot(pixel_coordinates);
    frag_color = vec4(color);
}