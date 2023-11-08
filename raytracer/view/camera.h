#ifndef CAMERA_H
#define CAMERA_H

#include "./../math/math.h"
#include "../utility/color.h"
#include "../world/world.h"

class Camera {
    public:
        float focal_length = 1.f;
        float viewport_height = 2.f;
        float viewport_width;
        Vector3df camera_center = {0.f, 0.f, 0.f};

        Camera(float height, float width) {
            this->viewport_width = this->viewport_height * double(width/height);
        }

        color cast_ray(Ray3df ray, World* world);
};


#endif