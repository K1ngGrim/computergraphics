#ifndef COLOR_H
#define COLOR_H

#include "./../math/math.h"
#include <SDL2/SDL.h>

class color {
    public:
        int colors[3];
    
        color() : colors{0, 0, 0} {}
        color(int e0, int e1, int e2) : colors{e0, e1, e2} {}
        color(double e0, double e1, double e2) : colors{static_cast<int>(e0), static_cast<int>(e1), static_cast<int>(e2)} {}

        int r() const { return colors[0]; }
        int g() const { return colors[1]; }
        int b() const { return colors[2]; }
};

void render_pixel(SDL_Renderer *renderer, color pixel_color, int x, int y) {
    SDL_SetRenderDrawColor(renderer, pixel_color.r(), pixel_color.g(), pixel_color.b(), 255);
    SDL_RenderDrawPoint(renderer, x, y);
}


#endif