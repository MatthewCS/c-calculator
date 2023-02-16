#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashset.h"

int hashset_hash(const HS* hashset, const char* str) {
  unsigned int sum = 0;
  const int length = strlen(str);

  for(int i = 0; i < length; ++i)
    sum += (int)str[i];

  return sum % hashset->capacity;
}

HS* new_hashset(char* items[], const unsigned int count)
{
  HS* set = (HS*) malloc(sizeof(HS));

  set->capacity = HS_DEFAULT_CAPACITY;
  set->items = (HSbucket**) calloc(set->capacity, sizeof(HSbucket*));

  for(int i = 0; i < count; ++i)
  {
    int item_index = hashset_hash(set, items[i]);
    HSbucket* new_bucket = (HSbucket*) malloc(sizeof(HSbucket));
    new_bucket->val = (char*) malloc(sizeof(items[i]) + 1);
    strcpy(new_bucket->val, items[i]);
    new_bucket->next = NULL;

    // scenario 1: bucket not populated (no hash collision)
    if(set->items[item_index] == NULL)
      set->items[item_index] = new_bucket;
    // scenario 2: bucket populated (hash collision)
    else
    {
      HSbucket* bucket = set->items[item_index];
      do
      {
        // scenario 2.1: item already found, abort
        if(strcmp(bucket->val, items[i]) == 0)
        {
          fprintf(stderr, "ERROR: item \"%s\" inserted into set twice. Aborting. . .", items[i]);
          exit(1);
        }

        if(bucket->next != NULL)
          bucket = bucket->next;
      }
      while(bucket->next != NULL);
    
      // scenario 2.2: append value to bucket
      bucket->next = new_bucket;
    }
  }

  return set;
}

void free_hashset(HS* hashset)
{
  for(int i = 0; i < hashset->capacity; ++i)
  {
    HSbucket* bucket = hashset->items[i];
    HSbucket* temp;

    while(bucket != NULL)
    {
      temp = bucket;
      bucket = bucket->next;
      free(temp);
    }
  }

  free(hashset->items);
  free(hashset);
}

int hashset_find(HS* hashset, const char* str)
{
  int item_index = hashset_hash(hashset, str);

  // scenario 1: no bucket
  if(hashset->items[item_index] == NULL)
    return 0;
  // scenario 2: bucket found
  else
  {
    HSbucket* bucket = hashset->items[item_index];
    
    while(bucket != NULL)
    {
      // scenario 2.1: item found
      if(strcmp(bucket->val, str) == 0)
        return 1;

      bucket = bucket->next;
    }

    // scenario 2.2: item not found
    return 0;
  }
}
