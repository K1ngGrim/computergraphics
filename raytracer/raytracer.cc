#undef main

#include "math.h"
#include "geometry/geometry.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "view/window.h"
#include "view/camera.h"
#include "utility/color.h"

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

Sphere3df* sphere;

color ray_color(const Ray3df r) {
  if(sphere->intersects(r)) {
    return color{1.f, 0.f, 0.f};
  }

  Vector3df unit_direction = r.direction;
  unit_direction.normalize();
  auto a = 0.5f*(unit_direction[1] + 1.0f);
  
  auto b = (1.0f-a)*color{1.f, 1.f, 1.f} + a*color{0.5f, 0.7f, 1.f};
  return b;
}

int main(void) {
  // Bildschirm erstellen

  const char* title = "Raytracer";
  const float width = 512.f;
  const float height = 512.f;

  Window* win = new Window(title, width, height);

  // Kamera erstellen

  Camera* cam = new Camera(width, height);

  printf("Viewport: %fH %fW", cam->viewport_height, cam->viewport_width);

  auto viewport_u = Vector3df{cam->viewport_width, 0, 0};
  auto viewport_v = Vector3df{0, -cam->viewport_height, 0};

  auto pixel_delta_u = viewport_u / width;
  auto pixel_delta_v = viewport_v / height;

  Vector3df viewport_upper_left = cam->camera_center - Vector3df{0, 0, cam->focal_length} - viewport_u/2.f - viewport_v/2.f;
  auto pixel00_loc = viewport_upper_left + (0.5f * (pixel_delta_u + pixel_delta_v));

  sphere = new Sphere3df({ 0.0f, 0.0f, -1.f }, 0.1f);

  // Für jede Pixelkoordinate x,y
  //   Sehstrahl für x,y mit Kamera erzeugen
  //   Farbe mit raytracing-Methode bestimmen
  //   Beim Bildschirm die Farbe für Pixel x,y, setzten

  win->Run();

  while (win->running) {
		win->PollEvents();
    
		for (int j = 0; j < win->height; ++j) {
      for (int i = 0; i < win->width; ++i) {

        Vector3df pixel_center = pixel00_loc + (float(i) * pixel_delta_u) + (float(j) * pixel_delta_v);
        Vector3df ray_direction = pixel_center - cam->camera_center;
        Ray3df ray = {cam->camera_center, ray_direction};

        //color pixel_color = ray_color(r);
        //write_color(std::cout, pixel_color);

        //auto p_color = color{float(i)/(win->width-1.f), float(j)/(win->height-1.f), 0.f};
        color p_color = ray_color(ray);
        render_pixel(win->renderer, p_color, i, j);
      }
    }
		SDL_RenderPresent(win->renderer);
	}
}

