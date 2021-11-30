#version 450

in vec4 gl_FragCoord;
in vec2 vUV;

out vec4 fCol;

uniform float width;

int iters() {
    float re = (gl_FragCoord.x / width - 0.5) * 4.0;
    float im = (gl_FragCoord.y / width - 0.7) * 4.0;

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
     }
     float iterations = float(its) / 500;
     return vec4(0.0, iterations, 0.0, 1.0);
}

void main() {
//    fCol = vec4(vCol, 1.0);
//    fCol = vec4(1.0, 0.0, 0.0, 1.0);
      fCol = mbrot_color();
}
