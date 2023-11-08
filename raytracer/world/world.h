#ifndef WORLD_H
#define WORLD_H

#include "../geometry/geometry.h"
#include "../utility/color.h"

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
    void add(WorldObject s);
    WorldObject get(int index);

    WorldObject* find_nearest_object(Ray3df ray);
};

#endif