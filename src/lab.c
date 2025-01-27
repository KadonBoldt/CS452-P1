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
  list->head = NULL;
  list->size = 0;
  return list;
}

void list_destroy(list_t **list) {
  if (list == NULL || *list == NULL) {
    return;
  }
  node_t *current = (*list)->head;
  while (current != NULL) {
    node_t *next = current->next;
    if ((*list)->destroy_data != NULL) {
      (*list)->destroy_data(current->data);
    }
    free(current);
    current = next;
  }
  free(*list);
  *list = NULL;
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
  node->next = list->head;
  node->prev = NULL;
  if (list->head != NULL) {
    node->prev = list->head->prev;
    node->prev->next = node;
    list->head->prev = node;
  }
  list->head = node;
  list->size++;
  return list;
}

void *list_remove_index(list_t *list, size_t index) {
  if (list == NULL || index >= list->size) {
    return NULL;
  }
  node_t *current = list->head;
  size_t i = 0;
  while (i < index && current != NULL) {
    current = current->next;
    i++;
  }
  if (current == NULL) {
    return NULL;
  }
  void *data = current->data;
  if (list->size == 1) {
    list->head = NULL;
  }
  else {
    if (i == 0) {
      list->head = current->next;
    }
    current->prev->next = current->next;
    current->next->prev = current->prev;
  }
  free(current);
  return data;
}

int list_indexof(list_t *list, void *data) {
  if (list == NULL || data == NULL || list->compare_to == NULL) {
    return -1;
  }
  node_t *current = list->head;
  size_t i = 0;
  while (i < list->size && current != NULL) {
    if (list->compare_to(current->data, data) == 0) {
      return i;
    }
    current = current->next;
    i++;
  }
  return -1;
}
