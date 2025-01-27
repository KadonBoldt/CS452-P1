#include <stdio.h>
#include <stdlib.h>
#include "lab.h"

list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {
  list_t *list = (list_t *)malloc(sizeof(list_t));
  if (list == NULL) {
    return NULL;
  }
  list->destroy_data = destroy_data;
  list->compare_to = compare_to;
  list->size = 0;
  node_t *node = (node_t *)malloc(sizeof(node_t));
  if (node == NULL) {
    free(list);
    return NULL;
  }
  node->data = NULL;
  node->next = node;
  node->prev = node;
  list->head = node;
  return list;
}

void list_destroy(list_t **list) {
  if (list == NULL || *list == NULL) {
    return;
  }
  node_t *current = (*list)->head;
  size_t i = 0;
  while (i <= (*list)->size) {
    node_t *next = current->next;
    if ((*list)->destroy_data != NULL && current->data != NULL) {
      (*list)->destroy_data(current->data);
    }
    free(current);
    current = next;
  }
  free(*list);
}

list_t *list_add(list_t *list, void *data) {
  if (list == NULL) {
    return NULL;
  }
  node_t *node = (node_t *)malloc(sizeof(node_t));
  if (node == NULL) {
    return NULL;
  }
  node->data = data;
  node->next = list->head->next;
  node->prev = list->head;
  node->next->prev = node;
  list->head->next = node;
  list->size++;
  return list;
}

void *list_remove_index(list_t *list, size_t index) {
  if (list == NULL || list->size < 1 || index > list->size || index <= 0) {
    return NULL;
  }
  node_t *current = list->head->next;
  size_t i = 1;
  while (i < index) {
    current = current->next;
    i++;
  }
  list->size--;
  void *data = current->data;
  current->prev->next = current->next;
  current->next->prev = current->prev;
  free(current);
  return data;
}

int list_indexof(list_t *list, void *data) {
  if (list == NULL || data == NULL || list->compare_to == NULL) {
    return -1;
  }
  node_t *current = list->head->next;
  size_t i = 1;
  while (i <= list->size) {
    if (list->compare_to(current->data, data) == 0) {
      return i;
    }
    current = current->next;
    i++;
  }
  return -1;
}
