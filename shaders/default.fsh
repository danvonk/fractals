#version 450

in vec4 gl_FragCoord;
in vec2 vUV;

out vec4 fCol;

uniform int width;
uniform int height;
uniform float zoom;
uniform int max_iters;
uniform float offsetX;
uniform float offsetY;

vec3 colorFunc(int iter) {
    vec3 color = vec3(0.01*iter , 1.0, 0.1+.4*(1.0+sin(0.3*iter)));
    // Convert from HSV to RGB
    // Taken from: http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 m = abs(fract(color.xxx + K.xyz) * 6.0 - K.www);
    return vec3(color.z * mix(K.xxx, clamp(m - K.xxx, 0.0, 1.0), color.y));
}


int iters() {
    float re = (gl_FragCoord.x / width) * zoom + offsetX;
    float im = (gl_FragCoord.y / height) * zoom + offsetY;

    float const_re = re;
    float const_im = im;

    int iterations = 0;
    for (; iterations < max_iters; iterations++) {
          float tmp_re = re;
          re = (re * re - im * im) + const_re;
          im = (2.0 * tmp_re * im) + const_im;
          float dist = re*re + im*im;
          if (dist > 4.0) {
             break;
          }
    }
    return iterations;
}

vec4 mbrot_color() {
     int its = iters();
     if (its == max_iters) {
        // in set
        gl_FragDepth = 0.0;
        return vec4(0.0, 0.0, 0.0, 1.0);
     } else {
         return vec4(colorFunc(its), 1.0);
     }
}

void main() {
      fCol = mbrot_color();
}
