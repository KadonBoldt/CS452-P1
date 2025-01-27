#include <stdio.h>
#include <stdlib.h>
#include "lab.h"

list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {
  list_t *list = (list_t *)malloc(sizeof(list_t));
  if (!list) {
    return NULL;
  }
  list->destroy_data = destroy_data;
  list->compare_to = compare_to;
  list->size = 0;
  list->head = (node_t *)malloc(sizeof(node_t));
  if (!list->head) {
    free(list);
    return NULL;
  }
  list->head->data = NULL;
  list->head->next = list->head;
  list->head->prev = list->head;
  return list;
}

void list_destroy(list_t **list) {
  if (!list || !(*list)) {
    return;
  }
  node_t *current = (*list)->head->next;
  while (current != (*list)->head) {
    node_t *next = current->next;
    (*list)->destroy_data(current->data);
    free(current);
    current = next;
  }
  free((*list)->head);
  free(*list);
  *list = NULL;
}

list_t *list_add(list_t *list, void *data) {
  if (!list || !data) {
    return list;
  }
  node_t *node = (node_t *)malloc(sizeof(node_t));
  if (!node) {
    return list;
  }
  node->data = data;
  node->next = list->head->next;
  node->prev = list->head;
  list->head->next->prev = node;
  list->head->next = node;
  list->size++;
  return list;
}

void *list_remove_index(list_t *list, size_t index) {
  if (!list || index >= list->size) {
    return NULL;
  }
  node_t *current = list->head->next;
  for (size_t i = 0; i < index; i++) {
    current = current->next;
  }
  void *data = current->data;
  current->prev->next = current->next;
  current->next->prev = current->prev;
  free(current);
  list->size--;
  return data;
}

int list_indexof(list_t *list, void *data) {
  if (!list || !data) {
    return -1;
  }
  node_t *current = list->head->next;
  size_t i = 0;
  while (current != list->head) {
    if (list->compare_to(current->data, data) == 0) {
      return i;
    }
    current = current->next;
    i++;
  }
  return -1;
}
