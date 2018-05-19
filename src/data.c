#include <stdio.h>
#include <stdlib.h>
#include "data.h"

/* Las definiciones de HEADER, ONTICK y ONSERVE determinan las salidas que
 * tendrá el algoritmo, por ejemplo para generar un csv con los datos:
#define HEADER() printf("time,work,used_ubers\n")
#define ONTICK() printf("%d,%d,%d\n", total_t, total_w, n)
 * para obtener la salida requerida: */
#define HEADER()
#define ONTICK() printf("%d %d\n", total_t, total_w)
#define ONSERVE()

/* Cuenta las lineas de un archivo. */
int count_lines (FILE *fp) 
{
  int count = 0;
  char c = '\0';
  while ((c = getc(fp)) != EOF)
    if (c == '\n') 
      count++;
  rewind(fp);
  return count;
}

/* Abre el archivo con ubers y los guarda en un array. 
 * en usize se guarda el tamaño del array creado. */
Uber **open_uber_file (const char *filename, int *usize)
{
  FILE *fp = fopen(filename, "r");
  int i, size = count_lines(fp);
  Uber **ubers = (Uber **) malloc(sizeof(Uber *) * size);
  for (i = 0; i < size; i++) {
    ubers[i] = (Uber *) malloc(sizeof(Uber));
    ubers[i]->use = 0;
    fscanf(fp, UBER_FORMAT, &(ubers[i]->pos.x), &(ubers[i]->pos.y));
  }
  fclose(fp);
  *usize = size;
  return ubers;
}

/* Abre el archivo con solicitudes y las guarda en un array
 * en rsize se guarda el tamaño del array creado. */
Request **open_request_file (const char *filename, int *rsize)
{
  FILE *fp = fopen(filename, "r");
  int i, size = count_lines(fp);
  Request **reqs = (Request **) malloc(sizeof(Request *) * size);
  for (i = 0; i < size; i++) {
    reqs[i] = (Request *) malloc(sizeof(Request));
    fscanf(fp, REQUEST_FORMAT, 
           &(reqs[i]->start.x), &(reqs[i]->start.y),
           &(reqs[i]->end.x), &(reqs[i]->end.y),
           &(reqs[i]->t));
  }
  fclose(fp);
  *rsize = size;
  return reqs;
}

/* Borra un array de ubers, como el creado por open_uber_file. */
void del_uber_array (Uber **uber, int size)
{
  int i;
  for (i = 0; i < size; free(uber[i++]));
  free(uber);
}

/* Borra un array de solicitudes, como el creado por open_request_file. */
void del_request_array (Request **req, int size)
{
  int i;
  for (i = 0; i < size; free(req[i++]));
  free(req);
}

/* Crea una lista de ubers. */
List *new_list ()
{
  List *list = (List *) malloc(sizeof(List));
  list->first = NULL;
  list->last = NULL;
  return list;
}

/* Borra una lista de ubers. */
void del_list (List *list)
{
  while (list->first) {
    remove_from_list(list, list->first);
  }
  free(list);
}

/* Agrega un uber a una lista de ubers. */
void add_to_list (List *list, Uber *uber)
{
  Item *item = (Item *) malloc(sizeof(Item));
  item->uber = uber;
  item->next = NULL;
  item->prev = list->last;
  if (list->last)
    list->last->next = item;
  list->last = item;
  if (!list->first)
    list->first = item;
}

/* Quita un uber de una lista de ubers. */
void remove_from_list (List *list, Item *item)
{
  if (item->prev)
    item->prev->next = item->next;
  else
    list->first = item->next;
  if (item->next)
    item->next->prev = item->prev;
  else
    list->last = item->prev;
  free(item);
}

/* Crea una instancia del problema de k-servidores con los datos de un archivo
 * de ubers 'uber_fn' y un archivo de solicitudes 'request_fn'. */
KServer *kserver_from_files (const char *uber_fn, const char *req_fn)
{
  KServer *p = (KServer *) malloc(sizeof(KServer));
  p->uber = open_uber_file(uber_fn, &(p->uber_len));
  p->req = open_request_file(req_fn, &(p->req_len));
  p->running = new_list();
  HEADER();
  return p;
}

/* Borra una instancia del problema de k-servidores. */
void del_kserver (KServer *p)
{
  del_list(p->running);
  del_uber_array(p->uber, p->uber_len);
  del_request_array(p->req, p->req_len);
  free(p);
}

/* Retorna la distancia entre dos puntos. */
int distance (Coord c1, Coord c2)
{
  return abs(c1.x - c2.x) + abs(c1.y - c2.y);
}

/* Asigna un uber para que sirva una solicitud. */
void serve (Request *req, Uber *uber, List *running)
{
  ONSERVE();
  uber->use = distance(uber->pos, req->start) + distance(req->start, req->end);
  uber->pos.x = req->end.x;
  uber->pos.y = req->end.y;
  add_to_list(running, uber);
}

/* Avanza el tiempo una unidad, todos los ubers en la lista 'running' se mueven,
 * los ubers que llegan a su destino salen de la lista, se retorna el total de
 * lo recorrido en este instante de tiempo para que sea sumado al trabajo total. */
int tick (List *running)
{
  int sum = 0;
  Item *cur = running->first, *tmp;
  while (cur != NULL) {
    cur->uber->use--;
    sum++;
    if (cur->uber->use == 0) {
      tmp = cur->next;
      remove_from_list(running, cur);
      cur = tmp;
    } else {
      cur = cur->next;
    }
  }
  return sum;
}

/* Ejecuta el problema de k-servidores para uber. */
void run_kserver (KServer *p)
{
  int i, last_t=0, n, total_w =0, total_t = 0;
  Uber *thisUber;
  for (i = 0; i < p->req_len; i++) {
    if (p->req[i]->t != last_t) {
      /* El tiempo pasa hasta que llega una solicitud. */
      for (; last_t < p->req[i]->t; last_t++) {
        n = tick(p->running);
        total_w += n;
        total_t++;
        ONTICK();
      }
    }
    /* El tiempo pasa mientras no hay ubers disponibles */
    while ( !(thisUber = p->selector(p->req[i], p->uber, p->uber_len)) ) {
      n = tick(p->running);
      total_w += n;
      total_t++;
      ONTICK();
    }
    serve(p->req[i], thisUber, p->running); // Se asigna la solicitud.
  }
  /* El tiempo pasa mientras ubers se muevan. */
  while ( (n = tick(p->running)) ) {
    total_t++;
    total_w += n;
    ONTICK();
  };

}

