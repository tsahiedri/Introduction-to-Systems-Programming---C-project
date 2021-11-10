#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Word_t{
    MapDataElement data;
    MapKeyElement key;
    struct Word_t *next;
}*Word;

struct Map_t{
    Word head;
    Word iterator;
    int map_size;
    copyMapDataElements copyDataElement;
    copyMapKeyElements copyKeyElement;
    freeMapDataElements freeDataElement;
    freeMapKeyElements freeKeyElement;
    compareMapKeyElements compareKeyElements;
};

Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements){
    if(copyDataElement == NULL || copyKeyElement == NULL || freeDataElement == NULL ||
        freeKeyElement == NULL ||compareKeyElements == NULL ){
            return NULL;
    }
    Map map =malloc(sizeof(*map));
    if (map==NULL){
        return NULL;
    }
    map->head= NULL;
    map->iterator = NULL;
    map->map_size =0;
    map->copyDataElement=copyDataElement;
    map->copyKeyElement=copyKeyElement;
    map->freeKeyElement=freeKeyElement;
    map->freeDataElement=freeDataElement;
    map->compareKeyElements=compareKeyElements;
    return map;
}

void mapDestroy(Map map)
{
    if (map == NULL)
    {
        return;
    }
    mapClear(map);
    Word to_release = map->head;
    Word next = NULL;
    while (to_release!=NULL)
    {
        next = to_release->next;
        free(to_release);
        to_release = next;
    }
    free(map);
}

Map mapCopy(Map map)
{
    if(map == NULL)
    {
        return NULL;
    }
    Map copy = mapCreate(map->copyDataElement, map->copyKeyElement,
                         map->freeDataElement, map->freeKeyElement, map->compareKeyElements);
    if (copy == NULL)
    {
        return NULL;
    }
    map->iterator=map->head;
    while(map->iterator != NULL)
    {
        if(mapPut(copy, map->iterator->key, map->iterator->data) != MAP_SUCCESS)
        {
            mapDestroy(copy);
            return NULL;
        }
        map->iterator =map->iterator->next;

    }
    return copy;
}

int mapGetSize(Map map)
{
    if (map == NULL)
    {
        return -1;
    }
    return map->map_size;
}

bool mapContains(Map map, MapKeyElement element)
{
    if (map==NULL || element == NULL)
    {
        return false;
    }
    Word self_iterator = map->head;
    while(self_iterator !=NULL)
    {
        if (map->compareKeyElements(element, self_iterator->key) == 0)
        {
            return true;
        }
        self_iterator=self_iterator->next;
    }
    return false;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement)
{
    if (map == NULL || keyElement == NULL || dataElement == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    Word last_word = NULL;
    map->iterator = map->head;
    while (map->iterator != NULL && map->compareKeyElements(keyElement, map->iterator->key) > 0)
    {
        last_word = map->iterator;
        map->iterator = map->iterator->next;
    }
    if (map->iterator == NULL || map->compareKeyElements(keyElement, map->iterator->key) < 0)
    {
        return mapMakeWord(map, last_word,keyElement,dataElement);
    }
    else
    {
        MapDataElement new_data = map->copyDataElement(dataElement);
        if (new_data == NULL)
        {
            return MAP_OUT_OF_MEMORY;
        }
        map->freeDataElement(map->iterator->data);
        map->iterator->data = new_data;
        return MAP_SUCCESS;
    }
}

MapDataElement mapGet(Map map, MapKeyElement keyElement)
{
    if (map == NULL || keyElement == NULL )
    {
        return NULL;
    }
    Word self_iterator = map->head;
    while(self_iterator != NULL)
    {
        if (map->compareKeyElements(keyElement, self_iterator->key) == 0)
        {
            return self_iterator->data;
        }
        self_iterator=self_iterator->next;
    }
    return NULL;
}

MapResult mapRemove(Map map, MapKeyElement keyElement) 
{
    if (map == NULL || keyElement == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    Word last = NULL;
    Word self_iterator = map->head;
    while (self_iterator != NULL) {
        if (map->compareKeyElements(keyElement, self_iterator->key) == 0)
        {
            map->freeDataElement(self_iterator->data);
            map->freeKeyElement(self_iterator->key);
            freeWord(map, last);
            return MAP_SUCCESS;
        }
        last = self_iterator;
        self_iterator = self_iterator->next;
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}

MapKeyElement mapGetFirst(Map map)
{
    if (map == NULL || map->head == NULL)
    {
        return NULL;
    }
    map->iterator=map->head;
    return map->copyKeyElement(map->head->key);
}

MapKeyElement mapGetNext(Map map) 
{
    if (map == NULL || map->iterator == NULL || map->iterator->next == NULL)
    {
        return NULL;
    }
    map->iterator = map->iterator->next;
    return map->copyKeyElement(map->iterator->key);
}

MapResult mapClear(Map map)
{
    if (map == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    while (map->head != NULL)
    {
        mapRemove(map, map->head->key);
    }
    return MAP_SUCCESS;
}

static void freeWord(Map map, Word last)
{
    if (last == NULL)
    {
        Word new_head = map->head->next;
        free(map->head);
        map->head = new_head;
    }
    else
    {
        Word last_new_next = last->next->next;
        free(last->next);
        last->next = last_new_next;
    }
    map->map_size--;
}

static MapResult mapMakeWord(Map map, Word last, MapKeyElement keyElement, MapDataElement dataElement)
{
    Word new_word = malloc(sizeof(*new_word));
    if (new_word == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }
    new_word->data = map->copyDataElement(dataElement);
    new_word->key = map->copyKeyElement(keyElement);
    if (new_word->data == NULL || new_word->key == NULL)
    {
        free(new_word->data);
        free(new_word->key);
        free(new_word);
        return MAP_OUT_OF_MEMORY;
    }
    if (last == NULL)
    {
        new_word->next = map->head;
        map->head = new_word;
    }
    else
    {
        new_word->next = last->next;
        last->next = new_word;
    }
    map->map_size++;
    return MAP_SUCCESS;
}






