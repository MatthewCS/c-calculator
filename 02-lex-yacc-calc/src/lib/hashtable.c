#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

int hashstr(const HT* hashtable, const char* str) {
  unsigned int sum = 0;
  const int length = strlen(str);

  for(int i = 0; i < length; ++i)
    sum += (int)str[i];

  return sum % hashtable->capacity;
}

HT* new_hashtable()
{
  HT* table = (HT*) malloc(sizeof(HT));

  table->size = 0;
  table->capacity = HT_DEFAULT_CAPACITY;
  table->items = (HTbucket**) calloc(table->capacity, sizeof(HTbucket*));

  return table;
}

void free_hashtable(HT* hashtable)
{
  for(int i = 0; i < hashtable->capacity; ++i)
  {
    HTbucket* bucket = hashtable->items[i];
    HTbucket* temp;

    while(bucket != NULL)
    {
      temp = bucket;
      bucket = bucket->next;
      free(temp);
    }
  }

  free(hashtable->items);
  free(hashtable);
}

void hashtable_upsert(HT* hashtable, const char* key, const long double value)
{
  HTitem* item = (HTitem*) malloc(sizeof(HTitem));
  item->key = (char*) malloc(sizeof(key) + 1);
  strcpy(item->key, key);
  item->value = value;
  int index = hashstr(hashtable, key);
  HTbucket* bucket = hashtable->items[index];

  // scenario 1: no hash collision
  if(bucket == NULL)
  {
    HTbucket* new_bucket = (HTbucket*) malloc(sizeof(HTbucket));
    new_bucket->node = item;
    new_bucket->next = NULL;
    
    hashtable->items[index] = new_bucket;
    ++hashtable->size;

    return;
  }
  // scenario 2: hash collision
  do
  {
    // scenario 2.1: key found, update value
    if(strcmp(bucket->node->key, key) == 0)
    {
      bucket->node->value = value;
      return;
    }

    if(bucket->next != NULL)
      bucket = bucket->next;
  }
  while(bucket->next != NULL);

  // scenario 2.2: key not found, create new item in bucket
  HTbucket* new_bucket = (HTbucket*) malloc(sizeof(HTbucket));
  new_bucket->node = item;
  new_bucket->next = NULL;
  bucket->next = new_bucket;
  ++hashtable->size;
}

HTfindres hashtable_find(HT* hashtable, const char* key) {
  int index = hashstr(hashtable, key);
  HTbucket* bucket = hashtable->items[index];
  HTfindres result = {.error = 0, .value = 0};

  // scenario 1: empty bucket
  if(bucket == NULL)
  {
    result.error = 1;
    return result;
  }
  // scenario 2: populated bucket
  while(1)
  {
    // scenario 2.1: key found in bucket
    if(strcmp(bucket->node->key, key) == 0)
    {
      result.value = bucket->node->value;
      return result;
    }
    // scenario 2.2: end of bucket reached with no key found
    else if(bucket->next == NULL)
    {
      result.error = 1;
      return result;
    }
    
    bucket = bucket->next;
  }
}
