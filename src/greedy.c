#include <stdio.h>
#include <stdlib.h>
#include "data.h"

int main (int argc, const char * args[])
{
  if (argc < 2) {
    printf("Modo de uso: ./uber request.dat\n");
    return EXIT_FAILURE;
  }
  char *request_fn = (char*) args[1];
  Uber **manana = NULL,
       **tarde = NULL,
       **noche = NULL;
  init_ubers(manana, tarde, noche);

  int i, req_len;
  Request **req = open_request_file(request_fn, &req_len);

  printf("%d\n", req_len);
  for (i = 0; i < req_len; i++) 
    printf("%hd %hd - %hd %hd - %d\n",
           req[i]->x0, req[i]->y0,
           req[i]->x1, req[i]->y1,
           req[i]->t);

  return EXIT_SUCCESS;
}
