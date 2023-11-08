#include "camera.h"
#include "../world/world.h"
#include "../utility/color.h"
#include "../geometry/geometry.h"
#include <iostream>
#include <fstream>
using namespace std;

color Camera::cast_ray(Ray3df ray, World* world) {
    WorldObject* obj = world->find_nearest_object(ray);
    if(obj != nullptr) {
        return obj->material;
    }
        

    //else return background-color
    Vector3df unit_direction = ray.direction;
    unit_direction.normalize();
    auto a = 0.5f*(unit_direction[1] + 1.0f);
    return (1.0f-a)*color{1.f, 1.f, 1.f} + a*color{0.5f, 0.7f, 1.f};
}