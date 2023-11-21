#undef main

#include <cmath>
#include "geometry/geometry.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "view/window.h"
#include "view/camera.h"
#include "utility/color.h"
#include "utility/light.h"
#include "world/world.h"

using namespace std;

// Die folgenden Kommentare beschreiben Datenstrukturen und Funktionen
// Die Datenstrukturen und Funktionen die weiter hinten im Text beschrieben sind,
// hängen höchstens von den vorhergehenden Datenstrukturen ab, aber nicht umgekehrt.



// Ein "Bildschirm", der das Setzen eines Pixels kapselt
// Der Bildschirm hat eine Auflösung (Breite x Höhe)
// Kann zur Ausgabe einer PPM-Datei verwendet werden oder
// mit SDL2 implementiert werden.



// Eine "Kamera", die von einem Augenpunkt aus in eine Richtung senkrecht auf ein Rechteck (das Bild) zeigt.
// Für das Rechteck muss die Auflösung oder alternativ die Pixelbreite und -höhe bekannt sein.
// Für ein Pixel mit Bildkoordinate kann ein Sehstrahl erzeugt werden.



// Für die "Farbe" benötigt man nicht unbedingt eine eigene Datenstruktur.
// Sie kann als Vector3df implementiert werden mit Farbanteil von 0 bis 1.
// Vor Setzen eines Pixels auf eine bestimmte Farbe (z.B. 8-Bit-Farbtiefe),
// kann der Farbanteil mit 255 multipliziert  und der Nachkommaanteil verworfen werden.


// Das "Material" der Objektoberfläche mit ambienten, diffusem und reflektiven Farbanteil.



// Ein "Objekt", z.B. eine Kugel oder ein Dreieck, und dem zugehörigen Material der Oberfläche.
// Im Prinzip ein Wrapper-Objekt, das mindestens Material und geometrisches Objekt zusammenfasst.
// Kugel und Dreieck finden Sie in geometry.h/tcc


// verschiedene Materialdefinition, z.B. Mattes Schwarz, Mattes Rot, Reflektierendes Weiss, ...
// im wesentlichen Variablen, die mit Konstruktoraufrufen initialisiert werden.


// Die folgenden Werte zur konkreten Objekten, Lichtquellen und Funktionen, wie Lambertian-Shading
// oder die Suche nach einem Sehstrahl für das dem Augenpunkt am nächsten liegenden Objekte,
// können auch zusammen in eine Datenstruktur für die gesammte zu
// rendernde "Szene" zusammengefasst werden.

// Die Cornelbox aufgebaut aus den Objekten
// Am besten verwendet man hier einen std::vector< ... > von Objekten.

// Punktförmige "Lichtquellen" können einfach als Vector3df implementiert werden mit weisser Farbe,
// bei farbigen Lichtquellen müssen die entsprechenden Daten in Objekt zusammengefaßt werden
// Bei mehreren Lichtquellen können diese in einen std::vector gespeichert werden.

// Sie benötigen eine Implementierung von Lambertian-Shading, z.B. als Funktion
// Benötigte Werte können als Parameter übergeben werden, oder wenn diese Funktion eine Objektmethode eines
// Szene-Objekts ist, dann kann auf die Werte teilweise direkt zugegriffen werden.
// Bei mehreren Lichtquellen muss der resultierende diffuse Farbanteil durch die Anzahl Lichtquellen geteilt werden.

// Für einen Sehstrahl aus allen Objekte, dasjenige finden, das dem Augenpunkt am nächsten liegt.
// Am besten einen Zeiger auf das Objekt zurückgeben. Wenn dieser nullptr ist, dann gibt es kein sichtbares Objekt.

// Die rekursive raytracing-Methode. Am besten ab einer bestimmten Rekursionstiefe (z.B. als Parameter übergeben) abbrechen.

void takeScreenshot(Window *win) {
    SDL_Surface *sshot = SDL_CreateRGBSurface(0, win->width, win->height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(win->renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);

    time_t now = time(0);
    std::string dt = ctime(&now);
    std::replace(dt.begin(), dt.end(), ':', '-');
    std::replace(dt.begin(), dt.end(), ' ', '-');

    dt = "./screenshots/screenshot" + dt;
    dt = dt + ".bmp";
    SDL_SaveBMP(sshot, dt.c_str());
    SDL_FreeSurface(sshot);
}

Camera* cam;

int main(void) {


    auto* win = new Window("Raytracer", 400.f, cam);

  // Kamera erstellen
    cam = new Camera(win->height, win->width);
    win->cam = cam;

  WorldObject obj1 = WorldObject({ { 10021.f, 0.0f, 0.f }, 10000.f },{{0.01f, 1.f, 0.01f}, 0});
  WorldObject obj2 = WorldObject({ { -10021.f, 0.0f, 0.f }, 10000.f }, {{1.f, 0.f, 0.f}, 0});
  WorldObject obj4 = WorldObject({ { 0.f, -10012.0f, 0.f }, 10000.f }, {{1.f, 1.f, 1.f}, 0});
  WorldObject obj5 = WorldObject({ { 0.f, 10012.0f, 0.f }, 10000.f }, {{1.f, 1.f, 1.f}, 0});
  WorldObject obj3 = WorldObject({ { 0.0f, 0.0f, -10030.f }, 10000.f }, {{1.f, 1.f, 1.f}, 0});

  WorldObject obj6 = WorldObject({ {3.f, -8.f, -15.f}, 1.f }, {{0.5f, 0.5f, 1.f}, 0});
  WorldObject obj7 = WorldObject({ {-14.f, -8.f, -17.f}, 3.f }, {{1.f, 1.f, 1.f}, 1});

  PointLight light = PointLight({5.f, 11.f, -18.f});

  auto world = new World();

  world->lights.push_back(light);
  world->add(obj6);
  world->add(obj1);
  world->add(obj2);
  world->add(obj3);
  world->add(obj4);
  world->add(obj5);
  world->add(obj7);

  printf("Viewport: %fW %fH \n", cam->viewport_width, cam->viewport_height);


  // Für jede Pixelkoordinate x,y
  //   Sehstrahl für x,y mit Kamera erzeugen
  //   Farbe mit raytracing-Methode bestimmen
  //   Beim Bildschirm die Farbe für Pixel x,y, setzten

  win->Run();
  bool tookScreenshot = false;
  while (win->running) {
      auto viewport_u = Vector3df{cam->viewport_width, 0, 0.f};
      auto viewport_v = Vector3df{0, -cam->viewport_height, 0.f};

      auto pixel_delta_u = viewport_u / win->width;
      auto pixel_delta_v = viewport_v / win->height;

      Vector3df viewport_upper_left = cam->camera_center - Vector3df{0.f, 0.f, cam->focal_length} - viewport_u/2.f - viewport_v/2.f;
      auto pixel00_loc = viewport_upper_left + (0.5f * (pixel_delta_u + pixel_delta_v));
      win->PollEvents();
      for (int j = 0; j < int(win->height); ++j) {
          for (int i = 0; i < int(win->width); ++i) {
            Vector3df pixel_center = pixel00_loc + (float(i) * pixel_delta_u) + (float(j) * pixel_delta_v);
            Vector3df ray_direction = pixel_center - cam->camera_center;
            Ray3df ray = {cam->camera_center, ray_direction};

            color p_color = cam->ray_color(ray, world, 10);
            render_pixel(win->renderer, p_color, i, j);
            }
        }
        SDL_RenderPresent(win->renderer);

        if(!tookScreenshot) {
            takeScreenshot(win);
            tookScreenshot = true;
        }
	}
}

