#include "camera.h"
#include <iostream>
using namespace std;

color Camera::cast_ray(Ray3df ray, World* world) {
    Intersection_Context<float, 3> context;
    WorldObject* obj = world->find_nearest_object(ray, context);
    if(obj != nullptr) {
        return this->lambertian(world->lights[0], context) * obj->material;
    }

    //else return background-color
    Vector3df unit_direction = ray.direction;
    unit_direction.normalize();
    auto a = 0.5f*(unit_direction[1] + 1.0f);
    return (1.0f-a)*color{1.f, 1.f, 1.f} + a*color{0.5f, 0.7f, 1.f};
}

float Camera::lambertian(Light light, Intersection_Context<float, 3> context) {
    //Gegeben Oberflächen normale ~n eines Oberflächenpunkts, Blickrichtung ~v zum
    //Auge, Richtung zur Lichtquelle ~l und Anteil diffusen Lichts kd.
    //Gesucht Intensität des Lichts (Farbe) L, die gesehen wird.
    //L = kd ·~l max{0, cos Θ} = kd ·~l max{0, ~n ·~l}
    //Ben: color * max(0, n * l) alles vektoren normieren

    //erstmal ohne schattenwurf von anderen objekten zwischen obj1 und licht;
    //Vektor zum licht = lichtpunkt - intersection

    Vector3df toLight = (light.center - context.intersection);
    toLight.normalize();
    Vector3df n = context.normal;
    n.normalize();

    float r = n * toLight;
    auto res = max(0.f, r);
    return res;

}

