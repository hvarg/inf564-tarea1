#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "data.h"

#define HEADER() printf("t,d,n\n")
//#define ONTICK() printf("%d %d (%d)\n", total_t, total_w, n)
#define ONTICK() printf("%d,%d,%d\n", total_t, total_w, n)
//#define ONTICK() 
//#define ONTICK() printf("%d,%d,%d\n", total_t, n,i)
#define ONSERVE()
//#define ONSERVE() printf("%d\n", thisUber->use);
//#define ONSERVE() printf("%d, %d\n", total_t, total_w);

double r2 ()
{
  return (double)rand() / (double)((unsigned)RAND_MAX + 1);
  //return (double)rand() / (double)RAND_MAX ;
}


Uber *harmonic (Request *req, Uber **uber, int uber_len, double *p)
{
  int j;
  double sum_p = 0.0, s=0.0, r=0.0;//, tmp=0.0;
  for (j = 0; j < uber_len; j++) {
    if (uber[j]->use == 0) {
      p[j] = 1.0 / distance(req->start, uber[j]->pos);
      sum_p += p[j];
    } else {
      p[j] = 0.0;
    }
  }
  if (sum_p > 0) {
    r = r2();
    for (j = 0; j < uber_len; j++) {
      /*tmp = p[j] / sum_p;
      s += tmp;
      printf("%f < %f\n", tmp, r);*/
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
  char *uber_fn = (char*) args[1],
       *request_fn = (char*) args[2];
  int uber_len, req_len;
  Uber **uber = open_uber_file(uber_fn, &uber_len);
  Request **req = open_request_file(request_fn, &req_len);

  srand(8);//time(NULL));
  HEADER();

  int i, last_t=0, n, total_w =0, total_t = 0;
  double *p = (double *) malloc(sizeof(double)*uber_len);
  Uber *thisUber;
  List *working = new_list();
  for (i = 0; i < req_len; i++) {
    if (req[i]->t != last_t) {
      for (; last_t < req[i]->t; last_t++) {
        n = work(working);
        total_w += n;
        total_t++;
        ONTICK();
      }
    }
    while ( !(thisUber = harmonic(req[i], uber, uber_len, p)) ) {
      n = work(working);
      total_w += n;
      total_t++;
      ONTICK();
    }
    serve(req[i], thisUber, working);
    ONSERVE();
  }
  while ( (n = work(working)) ) {
    total_t++;
    total_w += n;
    ONTICK();
  };
  del_list(working);
  del_uber_array(uber, uber_len);
  del_request_array(req, req_len);

  return EXIT_SUCCESS;
}
