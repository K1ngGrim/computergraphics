#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

class Window
{
public:
    SDL_Window *win;
    SDL_Renderer *renderer;
    bool running = true;
    SDL_Event event;

    float width, height;
    const char *windowTitle;

    // Konstruktor, Deklaration
    Window(const char *title, float w, float h)
    {
        this->windowTitle = title;
        this->width = w;
        this->height = h;
    }
    
    bool Init();
    int Run();
    void PollEvents();

    // Destruktor, Deklaration
    ~Window();
};

#endif
