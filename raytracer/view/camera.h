#ifndef CAMERA_H
#define CAMERA_H

#include "./../math/math.h"

class Camera {
    public:
        double focal_length = 1.0;
        double viewport_height = 2.0;
        double viewport_width;
        Vector3df camera_center = {0.0, 0.0, 0.0};

        Camera(int height, int width) {
            this->viewport_height * (static_cast<double>(width)/height);
        }
};


#endif