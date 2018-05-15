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
  rewind(fp);
  return count;
}

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

void del_uber_array (Uber **uber, int size)
{
  int i;
  for (i = 0; i < size; free(uber[i++]));
  free(uber);
}

void del_request_array (Request **req, int size)
{
  int i;
  for (i = 0; i < size; free(req[i++]));
  free(req);
}

List *new_list ()
{
  List *list = (List *) malloc(sizeof(List));
  list->first = NULL;
  list->last = NULL;
  return list;
}

void del_list (List *list)
{
  while (list->first) {
    remove_from_list(list, list->first);
  }
  free(list);
}

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

int distance (Coord c1, Coord c2)
{
  return abs(c1.x - c2.x) + abs(c1.y - c2.y);
}

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

void serve (Request *req, Uber *uber, List *working)
{
  //printf("(%d,%d) -> (%d,%d) -> (%d,%d) = ", uber->pos.x, uber->pos.y,
  //       req->start.x, req->start.y, req->end.x, req->end.y);
  uber->use = distance(uber->pos, req->start) + distance(req->start, req->end);
  uber->pos.x = req->end.x;
  uber->pos.y = req->end.y;
  //printf("%d \n", uber->use);
  add_to_list(working, uber);
}

int work (List *working)
{
  int sum = 0;
  Item *cur = working->first, *tmp;
  while (cur != NULL) {
    cur->uber->use--;
    sum++;
    if (cur->uber->use == 0) {
      tmp = cur->next;
      remove_from_list(working, cur);
      cur = tmp;
    } else {
      cur = cur->next;
    }
  }
  return sum;
}
