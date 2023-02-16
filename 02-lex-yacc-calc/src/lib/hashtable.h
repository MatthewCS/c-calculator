#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

// largest prime number less than 100
#define HT_DEFAULT_CAPACITY 97;

typedef struct HashtableItem
{
  char* key;
  long double value;
} HTitem;

// bucket is a linked list
typedef struct HashtableBucket
{
  HTitem* node;
  struct HashtableBucket* next;
} HTbucket;

typedef struct Hashtable
{
  int size;           // current size of the table
  int capacity;       // max number of buckets
  HTbucket** items;   // array of buckets
} HT;

typedef struct HashtableFindResult
{
  int error;          // 0 = no error, 1 = error
  long double value;
} HTfindres;

int hashstr(const HT* hashtable, const char* str);
HT* new_hashtable();
void free_hashtable(HT* hashtable);
void hashtable_upsert(HT* hashtable, const char* key, const long double value);
HTfindres hashtable_find(HT* hashtable, const char* key);

#endif
