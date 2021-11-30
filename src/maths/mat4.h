//
// Created by dan on 26/11/2021.
//

#ifndef FRACTALS_MAT4_H
#define FRACTALS_MAT4_H

#include <array>
#include <concepts>
#include "vec3.h"

namespace fr {
   template <std::floating_point T>
   class Mat4 {
       explicit Mat4();

   private:
      std::array<Vec3f, 4> mat_;
   };
}

#endif //FRACTALS_MAT4_H
