#version 450

in vec4 gl_FragCoord;
in vec2 vUV;

out vec4 fCol;

uniform float width;
uniform float height;
uniform float zoom;
uniform float runtime;

vec3 colorFunc(int iter) {
    // Color in HSV - Tweak these values to your liking and for different coloring effects
    vec3 color = vec3(0.012*iter , 1.0, 0.2+.4*(1.0+sin(0.3*iter)));

    // Convert from HSV to RGB
    // Taken from: http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 m = abs(fract(color.xxx + K.xyz) * 6.0 - K.www);
    return vec3(color.z * mix(K.xxx, clamp(m - K.xxx, 0.0, 1.0), color.y));
}


int iters() {
    float re = (gl_FragCoord.x / width - 0.5) * zoom * 4.0;
    float im = (gl_FragCoord.y / height - 0.7) * zoom * 4.0;
    //float re = (vUV.x / width) * zoom * 0.00000000002;
    //float im = (vUV.y / height) * zoom * 0.000000002;

    int iterations = 0;
    float const_re = re;
    float const_im = im;

    while (iterations < 500) {
          float tmp_re = re;
          re = (re * re - im * im) + const_re;
          im = (2.0 * tmp_re * im) + const_im;
          float dist = re*re + im*im;
          if (dist > 4.0) {
             break;
          }
          iterations++;
    }
    return iterations;
}

vec4 mbrot_color() {
     int its = iters();
     if (its == 500) {
        // in set
        gl_FragDepth = 0.0;
        return vec4(0.0, 0.0, 0.0, 1.0);
     } else {
         return vec4(colorFunc(its), 1.0);
     }

}

void main() {
//    fCol = vec4(vCol, 1.0);
//    fCol = vec4(1.0, 0.0, 0.0, 1.0);
      fCol = mbrot_color();
}
