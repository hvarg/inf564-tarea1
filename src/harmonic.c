#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "data.h"

/* Obtiene un número al azar entre 0 y 1. */
double r2 ()
{
  return (double)rand() / (double)((unsigned)RAND_MAX + 1);
}


double *p; // Global por eficiencia.
/* Asigna la probabilidad de que cada uber atienda la solicitud, luego elige un
 * número aleatorio y selecciona el uber indicado respecto a dicho número. */
Uber *harmonic (Request *req, Uber **uber, int uber_len)
{
  int j;
  double sum_p = 0.0, s=0.0, r=0.0;
  /* Obteniendo la suma. */
  for (j = 0; j < uber_len; j++) {
    if (uber[j]->use == 0) {
      p[j] = 1.0 / distance(req->start, uber[j]->pos);
      sum_p += p[j];
    } else {
      p[j] = 0.0;
    }
  }
  if (sum_p > 0) {
    r = r2(); //Número aleatorio
    /* Calcula las probabilidades de cada uber, selecciona segun 'r'. */
    for (j = 0; j < uber_len; j++) {
      s += p[j] / sum_p;
      if (s >= r)
        return uber[j];
    }
  }
  return NULL;
}

int main (int argc, const char * args[])
{
  if (argc < 3) {
    printf("Modo de uso: ./uber ubers.dat request.dat\n");
    return EXIT_FAILURE;
  }
  srand(8);//time(NULL)); //Semilla aleatoria.

  KServer *kserver = kserver_from_files ((char *) args[1], (char *) args[2]);
  p = (double *) malloc(sizeof(double)*kserver->uber_len);
  kserver->selector = &harmonic; //Elige el selector harmonic
  run_kserver(kserver);
  del_kserver(kserver);

  return EXIT_SUCCESS;
}
