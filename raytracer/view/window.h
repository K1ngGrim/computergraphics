#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

class Window
{
private:
    float aspect_ratio = 16.0f / 9.f;
public:
    SDL_Window *win{};
    SDL_Renderer *renderer{};
    bool running = true;
    SDL_Event event{};

    float width, height;
    const char *windowTitle;

    // Konstruktor, Deklaration
    Window(const char *title, float w)
    {
        this->windowTitle = title;
        this->width = w;
        this->height = float(w / this->aspect_ratio) < 1 ? 1 : float(w / this->aspect_ratio);
    }
    
    bool Init();
    int Run();
    void PollEvents();

    // Destruktor, Deklaration
    ~Window();
};

#endif
