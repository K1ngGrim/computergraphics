#undef main

#include "math.h"
#include "geometry/geometry.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "view/window.h"
#include "view/camera.h"
#include "utility/color.h"
#include "utility/helper.h"

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




void render() {

}

int main(void) {
  // Bildschirm erstellen

  const char* title = "Raytracer";
  const int width = 512;
  const int height = 512;

  Window* win = new Window(title, width, height);

  // Kamera erstellen

  Camera* cam = new Camera(width, height);

  auto viewport_u = Vector3df{Float(cam->viewport_width), 0, 0};
  auto viewport_v = Vector3df{0, Float(-cam->viewport_height), 0};

  auto pixel_delta_u = viewport_u / Float(width);
  auto pixel_delta_v = viewport_v / Float(height);

  Vector3df viewport_upper_left = cam->camera_center - Vector3df{0, 0, Float(cam->focal_length)} - viewport_u/Float(2) - viewport_v/Float(2);
  auto pixel00_loc = viewport_upper_left + Float(0.5) * (pixel_delta_u + pixel_delta_v);

  // Für jede Pixelkoordinate x,y
  //   Sehstrahl für x,y mit Kamera erzeugen
  //   Farbe mit raytracing-Methode bestimmen
  //   Beim Bildschirm die Farbe für Pixel x,y, setzten

  win->Run();

  while (win->running) {
		win->PollEvents();
    

		for (int j = 0; j < win->height; ++j) {
      for (int i = 0; i < win->width; ++i) {

        Vector3df pixel_center = pixel00_loc + (Float(i) * pixel_delta_u) + (Float(j) * pixel_delta_v);
        Vector3df ray_direction = pixel_center - cam->camera_center;
        Ray3df ray = {cam->camera_center, ray_direction};

        //color pixel_color = ray_color(r);
        //write_color(std::cout, pixel_color);

        auto pixel_color = color(double(i)/(win->width-1)*255.999, double(j)/(win->height-1)*255.999, double(0)*255.999);
        render_pixel(win->renderer, pixel_color, i, j);
      }
    }
		SDL_RenderPresent(win->renderer);
	}
}

