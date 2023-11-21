#ifndef LIGHT_H
#define LIGHT_H

#include "../math/math.h"
#include "color.h"

class Light {
private:
public:
    Vector3df center = {0.f, 0.f, 0.f};
    color lightColor = {0.984f, 0.945f, 0.725f};

protected:
    explicit Light(Vector3df center);
};

class PointLight : public Light {
public:
    explicit PointLight(Vector3df center) : Light(center) {

    };
};

#endif