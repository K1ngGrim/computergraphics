#ifndef WORLD_H
#define WORLD_H

#include "../geometry/geometry.h"
#include "../utility/color.h"
#include "../utility/light.h"

class WorldObject {
public:
    WorldObject(Sphere3df sphere, color c);
    WorldObject();
    color material = {0.f, 0.f, 0.f};
    Sphere3df sphere = Sphere3df({ 0.0f, 0.0f, 0.f }, 0.f);
};

class World {
public:
    World();

    std::vector<WorldObject> objects;
    std::vector<Light> lights;

    void add(WorldObject s);
    WorldObject get(int index);

    bool ray_intersects_any(Ray3df ray, Intersection_Context<float, 3> context);
    WorldObject* find_nearest_object(Ray3df ray, Intersection_Context<float, 3> &fc);
};

#endif