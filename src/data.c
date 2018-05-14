#include <stdio.h>
#include <stdlib.h>
#include "data.h"

int count_lines (FILE *fp) 
{
  int count = 0;
  char c = '\0';
  while ((c = getc(fp)) != EOF)
    if (c == '\n') 
      count++;
  //fseek(fp, 0, SEEK_SET);
  rewind(fp);
  return count;
}

Uber **open_uber_file (const char *filename, int size)
{
  FILE *fp = fopen(filename, "r");
  Uber **ubers = (Uber **) malloc(sizeof(Uber *) * size);
  int i;
  for (i = 0; i < size; i++) {
    ubers[i] = (Uber *) malloc(sizeof(Uber));
    fscanf(fp, UBER_FORMAT, &(ubers[i]->x), &(ubers[i]->y));
  }
  fclose(fp);
  return ubers;
}

Request **open_request_file (const char *filename, int *rsize)
{
  printf("reading: %s\n", filename);
  FILE *fp = fopen(filename, "r");
  int i, size = count_lines(fp);
  Request **reqs = (Request **) malloc(sizeof(Request *) * size);
  for (i = 0; i < size; i++) {
    reqs[i] = (Request *) malloc(sizeof(Request));
    fscanf(fp, REQUEST_FORMAT, 
           &(reqs[i]->x0), &(reqs[i]->y0),
           &(reqs[i]->x1), &(reqs[i]->y1),
           &(reqs[i]->t));
  }
  fclose(fp);
  *rsize = size;
  return reqs;
}

void init_ubers (Uber **manana, Uber **tarde, Uber **noche)
{
  manana = open_uber_file(MANANA_FN, MANANA_LEN);
  tarde  = open_uber_file(TARDE_FN, TARDE_LEN);
  noche  = open_uber_file(NOCHE_FN, NOCHE_LEN);
}
