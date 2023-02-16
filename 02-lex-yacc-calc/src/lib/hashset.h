#ifndef __HASHSET_H__
#define __HASHSET_H__

// largest prime number less than 100
#define HS_DEFAULT_CAPACITY 97;

// bucket is a linked list
typedef struct HashsetBucket
{
  char* val;
  struct HashsetBucket* next;
} HSbucket;

typedef struct Hashset
{
  int capacity;       // max number of buckets
  HSbucket** items;   // array of buckets
} HS;

int hashset_hash(const HS* hashset, const char* str);
HS* new_hashset(char* items[], const unsigned int count);
void free_hashset(HS* hashset);
int hashset_find(HS* hashset, const char* str);

#endif
