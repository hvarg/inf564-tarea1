#include <stdio.h>
#include <stdlib.h>
#include "data.h"

Uber *closest_uber (Request *req, Uber **ubers, int uber_len)
{
  int i, tmp, min = 2000;
  Uber *current = NULL;
  for (i = 0; i < uber_len; i++) {
    if (ubers[i]->use == 0) {
      tmp = distance(req->start, ubers[i]->pos);
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
  kserver->selector = &closest_uber;
  run_kserver(kserver);
  del_kserver(kserver);

  return EXIT_SUCCESS;
}
