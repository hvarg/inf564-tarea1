#ifndef DATAFILE_H_
#define DATAFILE_H_

#define UBER_FORMAT    "%hd %hd "
#define REQUEST_FORMAT "%hd %hd - %hd %hd - %d "

typedef struct {
  short int x, y;
} Coord;

typedef struct {
  Coord pos;
  int use;
} Uber;

typedef struct {
  Coord start, end;
  int t;
} Request;

typedef struct _item {
  Uber *uber;
  struct _item *next, *prev;
} Item;

typedef struct {
  Item *first, *last;
} List;

int     count_lines         (FILE *fp);
Uber    **open_uber_file    (const char *filename, int *size);
Request **open_request_file (const char *filename, int *size);
int     distance            (Coord c1, Coord c2);
Uber    *closest_uber       (Request *req, Uber **ubers, int uber_len);
void    serve               (Request *req, Uber *uber, List *working);
int     work                (List *working);
void del_uber_array (Uber **uber, int size);
void del_request_array (Request **req, int size);

List *new_list();
void del_list (List *list);
void add_to_list (List *list, Uber *uber);
void remove_from_list (List *list, Item *item);

#endif
/* vim: set ts=2 sw=2 sts=2 tw=80 : */
