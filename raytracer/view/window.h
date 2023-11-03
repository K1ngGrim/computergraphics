#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

class Window
{
public:
    SDL_Window *win;
    SDL_Renderer *renderer;

    int width, height;
    const char *windowTitle;

    // Konstruktor, Deklaration
    Window(const char *title, int w, int h)
    {
        this->windowTitle = title;
        this->width = w;
        this->height = h;
    }
    
    bool Init();
    int Run();

    // Destruktor, Deklaration
    ~Window();
};

#endif
