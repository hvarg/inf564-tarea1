#ifndef DATAFILE_H_
#define DATAFILE_H_

#define MANANA_FN "manana.dat"
#define TARDE_FN  "tarde.dat"
#define NOCHE_FN  "noche.dat"

#define MANANA_LEN 100
#define TARDE_LEN  200
#define NOCHE_LEN  50

#define UBER_FORMAT    "%hd %hd "
#define REQUEST_FORMAT "%hd %hd - %hd %hd - %d "

typedef struct {
  short int x, y;
} Uber;

typedef struct {
  short int x0, y0,
            x1, y1;
  int t;
} Request;

int     count_lines         (FILE *fp);
Uber    **open_uber_file    (const char *filename, int size);
Request **open_request_file (const char *filename, int *size);
void    init_ubers          (Uber **manana, Uber **tarde, Uber **noche);

#endif
/* vim: set ts=2 sw=2 sts=2 tw=80 : */
