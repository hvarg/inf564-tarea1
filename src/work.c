#include <stdio.h>
#include <stdlib.h>
#include "data.h"

/* Simplificación de la work function, en vez de tomar la configuración optima
 * offile con las solicitudes pasadas (NP), toma la diferencia entre la
 * configuración actual y la resultante de que la solicitud sea completada por
 * cada uber. */
Uber *semi_work (Request *req, Uber **ubers, int uber_len)
{
  int i, tmp, min = 2000;
  Uber *current = NULL;
  for (i = 0; i < uber_len; i++) {
    if (ubers[i]->use == 0) {
      tmp = distance(req->start, ubers[i]->pos) 
          + distance(ubers[i]->pos, req->end);
      if (tmp < min) {
        min = tmp;
        current = ubers[i];
      }
    }
  }
  return current;
}

int main (int argc, const char * args[])
{
  if (argc < 3) {
    printf("Modo de uso: ./uber ubers.dat request.dat\n");
    return EXIT_FAILURE;
  }
  KServer *kserver = kserver_from_files ((char *) args[1], (char *) args[2]);
  kserver->selector = &semi_work; // Elige el selector: work function.
  run_kserver(kserver);
  del_kserver(kserver);

  return EXIT_SUCCESS;
}
