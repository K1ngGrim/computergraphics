#ifndef COLOR_H
#define COLOR_H

#include "./../math/math.h"
#include <SDL2/SDL.h>

//Saving colors from 0F..1F
using color = Vector3df;

void render_pixel(SDL_Renderer *renderer, color pixel_color, int x, int y) {
    SDL_SetRenderDrawColor(renderer, pixel_color[0]*255.999, pixel_color[1]*255.999, pixel_color[2]*255.999, 255);
    SDL_RenderDrawPoint(renderer, x, y);
}

/**color ray_color(const Ray3df& r) {
    Vector3df unit_direction = r.direction.normalize();
    auto a = 0.5*(unit_direction[1] + Float(1));
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}**/


#endif