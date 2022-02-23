#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct TResult
{
  struct TResult * m_Next;
  int              m_ID;
  char           * m_Name;
} TRESULT;
#endif /* __PROGTEST__ */

typedef struct Person {
    int m_ID;
    unsigned int m_Hash;
    char *m_Name;
    struct Person *m_Parent1;
    struct Person *m_Parent2;
} TPERSON;

typedef struct Array {
    void **m_Data;
    int m_Size;
    int m_Capacity;
} TARRAY;

typedef struct Hash_map {
    TARRAY **m_Buckets;
    int m_Bucketcount;
    int m_UniqueCount;
} THASH_MAP;

typedef struct TDatabase {
    THASH_MAP *m_Hash_map;
} TDATABASE;

void initAll(TDATABASE *db);

void doneAll(TDATABASE *db);

int addPerson(TDATABASE *db,
              int id,
              const char *name,
              int id1,
              int id2);

TRESULT *ancestors(TDATABASE *db,
                   int id);

TRESULT *commonAncestors(TDATABASE *db,
                         int id1,
                         int id2);

void freeResult(TRESULT *res);

unsigned int hash_id(int ID) {
    return ID * 149 * 13 * 3;
}

unsigned int add_item_to_array(TARRAY *array, TPERSON *person) {
    if (array->m_Size == array->m_Capacity) {
        array->m_Capacity *= 2;
        array->m_Data = (void **) (TPERSON **) realloc(array->m_Data, sizeof(TPERSON *) * array->m_Capacity);
    }
    array->m_Data[array->m_Size] = person;
    array->m_Size++;
    return array->m_Size - 1;
}


TARRAY *create_array(int capacity) {
    TARRAY *array = (TARRAY *) malloc(sizeof(TARRAY));
    array->m_Size = 0;
    array->m_Capacity = capacity;
    array->m_Data = (void **) malloc(sizeof(void *) * capacity);
    return array;
}

void resize_hash_map(THASH_MAP *map) {
    if (map->m_UniqueCount / ((double) map->m_Bucketcount) < 0.5) {
        return;
    }
    TARRAY **old_items = map->m_Buckets;
    int new_bucket_size = map->m_Bucketcount * 2;
    map->m_Buckets = (TARRAY **) malloc(sizeof(TARRAY) * new_bucket_size);
    for (int i = 0; i < new_bucket_size; i++) {
        map->m_Buckets[i] = create_array(1);
    }
    for (int i = 0; i < map->m_Bucketcount; ++i) {
        TARRAY *bucket = old_items[i];
        for (int j = 0; j < bucket->m_Size; ++j) {
            TPERSON *person = (TPERSON *) bucket->m_Data[j];
            unsigned int index = person->m_Hash % (new_bucket_size);
            add_item_to_array(map->m_Buckets[index], person);
        }
        free(bucket->m_Data);
        free(bucket);
    }
    map->m_Bucketcount = new_bucket_size;
    free(old_items);
}

int add_person(THASH_MAP *map, TPERSON *person) {
    unsigned int hash = hash_id(person->m_ID);
    unsigned int bucket_number = hash % map->m_Bucketcount;
    TARRAY *bucket = map->m_Buckets[bucket_number];
    for (int i = 0; i < bucket->m_Size; ++i) {
        TPERSON *current = (TPERSON *) bucket->m_Data[i];
        if (current->m_ID == person->m_ID) {
            return 0;
        }
    }
    add_item_to_array(bucket, person);
    map->m_UniqueCount++;
    return 42;
}

TPERSON *get_person(THASH_MAP *map, int ID) {

    unsigned int hash = hash_id(ID);
    unsigned int bucket_number = hash % map->m_Bucketcount;
    TARRAY *bucket = map->m_Buckets[bucket_number];
    for (int i = 0; i < bucket->m_Size; ++i) {
        TPERSON *current = (TPERSON *) bucket->m_Data[i];
        if (current->m_ID == ID) {
            return current;
        }
    }
    return NULL;
}

THASH_MAP *create_hash_map() {
    THASH_MAP *map = (THASH_MAP *) malloc(sizeof(THASH_MAP));
    map->m_Bucketcount = 8;
    map->m_Buckets = (TARRAY **) malloc(sizeof(TARRAY *) * map->m_Bucketcount);
    for (int i = 0; i < map->m_Bucketcount; ++i) {
        map->m_Buckets[i] = create_array(1);
    }
    map->m_UniqueCount = 0;
    return map;
}

void initAll(TDATABASE *db) {
    db->m_Hash_map = create_hash_map();
}

void free_person(TPERSON *person) {
    free(person->m_Name);
    free(person);
}

void free_array(TARRAY *array,int free_all) {
    if(free_all){
        for (int i = 0; i < array->m_Size; ++i) {
            free_person((TPERSON *) array->m_Data[i]);
        }
    }
    free(array->m_Data);
    free(array);
}

void free_hash_map(THASH_MAP *map,int free_all) {
    for (int i = 0; i < map->m_Bucketcount; ++i) {
        free_array(map->m_Buckets[i],free_all);
    }
    free(map->m_Buckets);
    free(map);
}

void doneAll(TDATABASE *db) {
    free_hash_map(db->m_Hash_map,1);
}

int addPerson(TDATABASE *db,
              int id,
              const char *name,
              int id1,
              int id2) {
    if (id == 0) return 0;
    TPERSON *person = (TPERSON *) malloc(sizeof(TPERSON));
    person->m_ID = id;
    person->m_Name = (char*) malloc(sizeof(char)* (strlen(name)+1));
    strcpy(person->m_Name,name);
    if (id1 == 0) {
        person->m_Parent1 = NULL;
    } else {
        person->m_Parent1 = get_person(db->m_Hash_map, id1);
        if (person->m_Parent1 == NULL) {
            free_person(person);
            return 0;
        }
    }
    if (id2 == 0) {
        person->m_Parent2 = NULL;
    } else {
        person->m_Parent2 = get_person(db->m_Hash_map, id2);
        if (person->m_Parent2 == NULL) {
            free_person(person);
            return 0;
        }
    }
    if (person->m_Parent1 != NULL && person->m_Parent1 == person->m_Parent2) {
        free_person(person);
        return 0;
    }
    if (add_person(db->m_Hash_map, person)) {
        return 1;
    }
    free_person(person);
    return 0;
}

void add_ancestors(THASH_MAP *map, TPERSON *tperson) {
    if (tperson != NULL) {
        add_person(map, tperson);
        add_ancestors(map, tperson->m_Parent1);
        add_ancestors(map, tperson->m_Parent2);
    } else {
        return;
    }
}

TRESULT *create_result(int ID, char *name) {
    TRESULT *result = (TRESULT *) malloc(sizeof(TRESULT));
    result->m_ID = ID;
    result->m_Next = NULL;
    result->m_Name = (char*) malloc(sizeof(char)* (strlen(name)+1));
    strcpy(result->m_Name,name);
    return result;
}

TRESULT *hash_map_to_linked_list(THASH_MAP *map) {
    TRESULT *result = NULL;
    for (int i = 0; i < map->m_Bucketcount; ++i) {
        TARRAY *bucket = map->m_Buckets[i];
        for (int k = 0; k < bucket->m_Size; ++k) {
            TPERSON *current = (TPERSON *) bucket->m_Data[k];
            TRESULT *old_result = result;
            result = create_result(current->m_ID, current->m_Name);
            result->m_Next = old_result;
        }
    }
    return result;
}

TRESULT *ancestors(TDATABASE *db,
                   int id) {
    TPERSON *person = get_person(db->m_Hash_map, id);
    if (person == NULL || (person->m_Parent1 == NULL && person->m_Parent2 == NULL)) return NULL;
    THASH_MAP *map = create_hash_map();
    add_ancestors(map, person->m_Parent2);
    add_ancestors(map, person->m_Parent1);
    TRESULT *result=hash_map_to_linked_list(map);
    free_hash_map(map,0);
    return result;
}


TRESULT *commonAncestors(TDATABASE *db,
                         int id1,
                         int id2) {
    TPERSON *person1 = get_person(db->m_Hash_map, id1);
    if (person1 == NULL || (person1->m_Parent1 == NULL && person1->m_Parent2 == NULL)) return NULL;

    TPERSON *person2 = get_person(db->m_Hash_map, id2);
    if (person2 == NULL || (person2->m_Parent1 == NULL && person2->m_Parent2 == NULL)) return NULL;

    THASH_MAP *map1 = create_hash_map();
    add_ancestors(map1, person1->m_Parent1);
    add_ancestors(map1, person1->m_Parent2);

    THASH_MAP *map2 = create_hash_map();
    add_ancestors(map2, person2->m_Parent1);
    add_ancestors(map2, person2->m_Parent2);


    TRESULT *result = NULL;
    for (int i = 0; i < map1->m_Bucketcount; ++i) {
        TARRAY *bucket = map1->m_Buckets[i];
        for (int k = 0; k < bucket->m_Size; ++k) {
            TPERSON *current = (TPERSON *) bucket->m_Data[k];
            if(get_person(map2,current->m_ID)!=NULL){
                TRESULT *old_result = result;
                result = create_result(current->m_ID, current->m_Name);
                result->m_Next = old_result;
            }
        }
    }
    free_hash_map(map1,0);
    free_hash_map(map2,0);
    return result;
}

void freeResult(TRESULT *res) {
    TRESULT *current = res;
    TRESULT *next;
    while (current != NULL) {
        next = current->m_Next;
        free(current->m_Name);
        free(current);
        current = next;
    }
}
