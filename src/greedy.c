#include <stdio.h>
#include <stdlib.h>
#include "data.h"

int main (int argc, const char * args[])
{
  if (argc < 3) {
    printf("Modo de uso: ./uber ubers.dat request.dat\n");
    return EXIT_FAILURE;
  }
  char *uber_fn = (char*) args[1],
       *request_fn = (char*) args[2];
  int uber_len, req_len;
  Uber **uber = open_uber_file(uber_fn, &uber_len);
  Request **req = open_request_file(request_fn, &req_len);

  int i, last_t=0, n, total_w =0, total_t = 0;
  Uber *thisUber;
  List *working = new_list();
  for (i = 0; i < req_len; i++) {
    if (req[i]->t != last_t) {
      for (; last_t < req[i]->t; last_t++) {
        total_w += work(working);
        total_t++;
      }
      printf("%d %d\n", total_t, total_w);
    }
    while ( !(thisUber = closest_uber(req[i], uber, uber_len)) ) {
      total_w += work(working);
      total_t++;
    }
    if ((thisUber = closest_uber(req[i], uber, uber_len))) {
      serve(req[i], thisUber, working);
    };
  }
  while ( (n = work(working)) ) {
    total_t++;
    total_w += n;
  };
  del_list(working);
  del_uber_array(uber, uber_len);
  del_request_array(req, req_len);

  //printf("%d\n", req[i]->t);
  /*
  for (i = 0; i < req_len; i++) 
    printf("%hd %hd - %hd %hd - %d\n",
           req[i]->x0, req[i]->y0,
           req[i]->x1, req[i]->y1,
           req[i]->t);
  */

  return EXIT_SUCCESS;
}
