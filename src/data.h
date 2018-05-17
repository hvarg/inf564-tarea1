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

typedef struct {
  Uber **uber;
  Request **req;
  int uber_len, req_len;
  List *running;
  Uber *(*selector) (Request *req, Uber **uber, int uber_len);
} KServer;

int     count_lines         (FILE *fp);
Uber    **open_uber_file    (const char *filename, int *size);
Request **open_request_file (const char *filename, int *size);
void    del_uber_array      (Uber **uber, int size);
void    del_request_array   (Request **req, int size);

KServer *kserver_from_files (const char *uber_fn, const char *req_fn);
void    del_kserver         (KServer *p);
void    run_kserver         (KServer *p);

List    *new_list           ();
void    del_list            (List *list);
void    add_to_list         (List *list, Uber *uber);
void    remove_from_list    (List *list, Item *item);

int     distance            (Coord c1, Coord c2);
void    serve               (Request *req, Uber *uber, List *running);
int     tick                (List *running);
Uber    *closest_uber       (Request *req, Uber **ubers, int uber_len);


#endif
/* vim: set ts=2 sw=2 sts=2 tw=80 : */
