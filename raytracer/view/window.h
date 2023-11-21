#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include "camera.h"
#include "viewport.h"

class Window
{
private:
    float aspect_ratio = 16.0f / 9.f;
public:
    SDL_Window *win{};
    SDL_Renderer *renderer{};
    bool running = true;
    SDL_Event event{};
    Camera* cam;
    Viewport* viewport;
    float width, height;
    const char *windowTitle;

    // Konstruktor, Deklaration
    Window(const char *title, float w, Camera* cam)
    {
        this->windowTitle = title;
        this->width = w;
        this->height = float(w / this->aspect_ratio) < 1 ? 1 : float(w / this->aspect_ratio);
        this->cam = cam;
        this->viewport = new Viewport(*cam, this->width, this->height);
    }
    
    bool Init();
    int Run();
    void PollEvents();

    // Destruktor, Deklaration
    ~Window();
};

#endif
