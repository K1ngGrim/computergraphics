#include "color.h"

void render_pixel(SDL_Renderer *renderer, color pixel_color, int x, int y) {
    SDL_SetRenderDrawColor(renderer, pixel_color[0]*255.999, pixel_color[1]*255.999, pixel_color[2]*255.999, 255);
    SDL_RenderDrawPoint(renderer, x, y);
}