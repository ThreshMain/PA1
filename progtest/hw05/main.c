#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define ARRAY_RESIZE_FACTOR 2
#define HASH_MAP_LOAD_FACTOR 0.75
#define HASH_RESIZE_FACTOR 2

typedef struct {
    char name[100];
    // Full hash of name for faster resize of the hash map
    unsigned int hash;
    int count;
    // Initial value being -1 as not in the common_array
    int index_in_common_array;
} item_t;

typedef struct {
    // Number of items in the array
    int size;
    // Actual size of the array should be always bigger or equal to size
    int capacity;
    // Array of pointers to buckets
    item_t **items;
} item_array_t;

// HashMap with item name as key and count as value
typedef struct {
    item_array_t **buckets;
    int bucket_size;
    // Unique count to be able to resize the hash map if needed
    int unique_count;
} hash_map_t;

typedef struct {
    // Number of items in the array
    int size;
    // Actual size of the array should be always bigger or equal to size
    int capacity;
    int *data;
} int_array_t;

/**
 * Array to store the most common items in ordered list
 * With indexes to the beginning of specific count part of the array
 */
typedef struct {
    item_array_t *records;
    int_array_t *indexes;
} common_array_t;

/**
 * Calculates the hash of the string
 * @param str null terminated string
 * @return hash
 */
unsigned int hash_string(char *str) {
    unsigned int hash = 149;
    char c;
    while ((c = *str++)) {
        hash = c + (hash << 6);
    }
    return hash;
}

/**
 * Allocates memory for the item
 * and sets default values
 * @param name null terminated string max 99 chars long string
 * @param hash hash that should be used for hash map
 * @return item
 */
item_t *create_item(char *name, unsigned int hash) {
    item_t *item = (item_t *) malloc(sizeof(item_t));
    strcpy(item->name, name);
    item->hash = hash;
    item->count = 1;
    item->index_in_common_array = -1;
    return item;
}

/**
 * Allocates memory for the item array
 * and sets default values
 * @param capacity initial capacity of the array
 * @return item_array
 */
item_array_t *create_item_array(int capacity) {
    item_array_t *array = (item_array_t *) malloc(sizeof(item_array_t));
    array->size = 0;
    array->capacity = capacity;
    array->items = (item_t **) malloc(sizeof(item_t *) * capacity);
    return array;
}

/**
 * Adds item to the end of the array,
 * and if needed resizes the array by multiplying capacity by ARRAY_RESIZE_FACTOR
 * @param array initialized array
 * @param item not NULL item that should be added
 * @return index of the added item
 */
unsigned int add_item_to_array(item_array_t *array, item_t *item) {
    if (array->size == array->capacity) {
        array->capacity *= ARRAY_RESIZE_FACTOR;
        array->items = (item_t **) realloc(array->items, sizeof(item_t *) * array->capacity);
    }
    array->items[array->size] = item;
    array->size++;
    return array->size - 1;
}

/**
 * Adds int to the end of the array,
 * and if needed resizes the array by multiplying capacity by ARRAY_RESIZE_FACTOR
 * @param array initialized array
 * @param value int
 */
void add_int_to_array(int_array_t *array, int value) {
    if (array->size == array->capacity) {
        array->capacity *= ARRAY_RESIZE_FACTOR;
        array->data = (int *) realloc(array->data, sizeof(int) * array->capacity);
    }
    array->data[array->size] = value;
    array->size++;
}

/**
 * Free memory of the item array,
 * and all items in it.
 * @param array initialized array
 */
void free_item_array(item_array_t *array) {
    for (int i = 0; i < array->size; ++i) {
        free(array->items[i]);
    }
    free(array->items);
    free(array);
}

void free_int_array(int_array_t *array) {
    free(array->data);
    free(array);
}

/**
 * Resizes the hash map by factor of HASH_RESIZE_FACTOR, if the load factor is bigger than HASH_MAP_LOAD_FACTOR
 * @param map initialized hash map
 */
void resize_hash_map(hash_map_t *map) {
    if (map->unique_count / ((double) map->bucket_size) < HASH_MAP_LOAD_FACTOR) {
        return;
    }
    item_array_t **old_items = map->buckets;
    int new_bucket_size = map->bucket_size * HASH_RESIZE_FACTOR;
    map->buckets = (item_array_t **) malloc(sizeof(item_array_t *) * new_bucket_size);
    for (int i = 0; i < new_bucket_size; i++) {
        map->buckets[i] = create_item_array(1);
    }
    for (int i = 0; i < map->bucket_size; ++i) {
        item_array_t *bucket = old_items[i];
        for (int j = 0; j < bucket->size; ++j) {
            item_t *item = bucket->items[j];
            unsigned int index = item->hash % (new_bucket_size);
            add_item_to_array(map->buckets[index], item);
        }
        free(bucket->items);
        free(bucket);
    }
    map->bucket_size = new_bucket_size;
    free(old_items);
}

/**
 * Adds 1 to the count of the item if it already exists in the hash map
 * otherwise creates and adds the item to the hash map
 * @param map initialized hash map
 * @param name name of the item
 * @return item that was added or already existed
 */
item_t *add_item(hash_map_t *map, char *name) {
    unsigned int hash = hash_string(name);
    unsigned int bucket_number = hash % map->bucket_size;
    item_array_t *bucket = map->buckets[bucket_number];
    for (int i = 0; i < bucket->size; ++i) {
        if (bucket->items[i]->hash == hash && strcmp(bucket->items[i]->name, name) == 0) {
            bucket->items[i]->count++;
            return bucket->items[i];
        }
    }
    item_t *item = create_item(name, hash);
    add_item_to_array(bucket, item);
    map->unique_count++;
    return bucket->items[bucket->size - 1];
}

/**
 * Creates hash map and initializes the buckets
 * @param bucket_size initial size of the hash map
 * @return new hash map
 */
hash_map_t *create_hash_map(int bucket_size) {
    hash_map_t *map = (hash_map_t *) malloc(sizeof(hash_map_t));
    map->bucket_size = bucket_size;
    map->buckets = (item_array_t **) malloc(sizeof(item_array_t *) * bucket_size);
    for (int i = 0; i < bucket_size; ++i) {
        map->buckets[i] = create_item_array(1);
    }
    return map;
}

/**
 * Frees the hash map and all its items and buckets
 * @param map
 */
void free_hash_map(hash_map_t *map) {
    for (int i = 0; i < map->bucket_size; ++i) {
        free_item_array(map->buckets[i]);
    }
    free(map->buckets);
    free(map);
}

/**
 * Creates common_array and initializes the arrays
 * @param capacity initial size of the common array
 * @return new common array
 */
common_array_t *create_common_array(int capacity) {
    common_array_t *array = (common_array_t *) malloc(sizeof(common_array_t));
    array->records = create_item_array(capacity);
    array->indexes = (int_array_t *) malloc(sizeof(int_array_t));
    array->indexes->size = 0;
    array->indexes->capacity = capacity;
    array->indexes->data = (int *) malloc(sizeof(int) * capacity);
    return array;
}

/**
 * Free memory of the common array,
 * does not free the items in it.
 * @param map initialized common array
 */
void free_common_array(common_array_t *map) {
    free_int_array(map->indexes);
    free(map->records->items);
    free(map->records);
    free(map);
}

/**
 * Swap 2 items in the item array
 * and also swap their index_in_common_array
 * @param array initialized item array containing both items
 * @param from index of the first item
 * @param to index of the second item
 */
void swap_items(item_array_t *array, int from, int to) {
    array->items[from]->index_in_common_array = to;
    array->items[to]->index_in_common_array = from;
    item_t *temp = array->items[from];
    array->items[from] = array->items[to];
    array->items[to] = temp;
}

/**
 * Returns the value on index,
 * if array does not have that index,
 * it will add 0 to the end until it has that index
 * @param map initialized int array
 * @param index index of the value
 * @return int on index
 */
int get_int_from_int_array(int_array_t *array, int index) {
    while (array->size <= index) {
        add_int_to_array(array, 0);
    }
    return array->data[index];
}

void update_common_items(common_array_t *map, item_t *item, int number_of_items) {
    if (map->records->size == 0) {
        item->index_in_common_array = map->records->size;
        add_item_to_array(map->records, item);
        add_int_to_array(map->indexes, map->records->size - 1);
        return;
    }

    if (item->index_in_common_array == -1) {
        int last_index = get_int_from_int_array(map->indexes, map->indexes->size - 1);
        if (last_index < number_of_items ||
            map->records->items[last_index]->count == item->count) {
            item->index_in_common_array = map->records->size;
            add_item_to_array(map->records, item);
        }
        return;
    }
    int from = item->index_in_common_array;
    int to = get_int_from_int_array(map->indexes, item->count - 2);
    if (from != to) {
        swap_items(map->records, from, to);
    }
    get_int_from_int_array(map->indexes, item->count - 1);
    int previous_count = item->count - 2;
    map->indexes->data[previous_count]++;
}

/**
 * Reads input in in format of:
 * space, up to 99 chars and new line
 * from user, logs the record and updates most common items.
 * @param map to log the record
 * @param common_items to log the most common items
 * @param number_of_items the number of most common items to log
 * @return 3 if every thing worked, 0 on user input error, EOF if stream is at the end of file
 */
int add_log_record(hash_map_t *map, common_array_t *common_items, int number_of_items) {
    char name[100];
    char space, new_line;
    int result = scanf("%c%99s%c", &space, name, &new_line);
    if (result != 3 || space != ' ' || new_line != '\n') {
        if (result == EOF) return EOF;
        return 0;
    }
    resize_hash_map(map);
    item_t *item = add_item(map, name);
    update_common_items(common_items, item, number_of_items);
    return result;
}

/**
 * Prints the desired number of most common items that were logged
 * @param map common_array
 * @param number_of_items number of desired items to be printed
 */
void print_most_common_items(common_array_t *map, int number_of_items) {
    int position = 1;
    for (int i = map->indexes->size - 1; i >= 0; i--) {
        int index = map->indexes->data[i];
        int next_index = i >= 1 ? map->indexes->data[i - 1] : map->records->size;
        int count = next_index - index;
        if (index >= number_of_items) break;
        if (count > 1) {
            for (int j = 0; j < count; ++j) {
                printf("%d.-%d. %s, %dx\n", position, position + count - 1,
                       map->records->items[index + j]->name,
                       map->records->items[index + j]->count);
            }
        } else if (count > 0) {
            printf("%d. %s, %dx\n", position, map->records->items[index]->name,
                   map->records->items[index]->count);
        }
        position += count;
    }
}

/**
 * Counts the number of common items inside the array
 * @param map common_array
 * @param number_of_items number of desired items to be count
 * @return int sum
 */
int common_items_sum(common_array_t *map, int number_of_items) {
    int sum = 0;
    for (int i = map->indexes->size - 1; i >= 0; i--) {
        int index = map->indexes->data[i];
        int next_index = i >= 1 ? map->indexes->data[i - 1] : map->records->size;
        int count = next_index - index;
        if (index >= number_of_items) break;
        sum += (i + 1) * count;
    }
    return sum;
}

/**
 * Execute specified operation based on the char operation
 * @param operation specifies the operation to be executed
 * @param map initialised hash_map to store logs
 * @param common_items most common items
 * @param number_of_items desired number of most common items to be logged
 * @return status of the operation 1 - success, 0 - failed, EOF(-1) - failed with end of stream
 */
int execute_operation(char operation, hash_map_t *map, common_array_t *common_items, int number_of_items) {
    int result;
    switch (operation) {
        case '+':
            result = add_log_record(map, common_items, number_of_items);
            break;
        case '#':
            print_most_common_items(common_items, number_of_items);
        case '?':
            printf("Nejprodavanejsi zbozi: prodano %d kusu\n", common_items_sum(common_items, number_of_items));
            result = 1;
            break;
        default:
            return 0;
    }
    if (result > 1) return 1;
    return result;
}

/**
 * Interacts with user to load logs and perform all necessary actions desired by user:
 * save log, print most common items, print sum of most common items
 * @return status 1 - success, 0 - fail
 */
int parseLog() {
    hash_map_t *map = create_hash_map(128);
    int result;
    int number_of_items = 0;
    char operation;
    printf("Pocet sledovanych:\n");

    if ((result = scanf("%d", &number_of_items)) == 1 && number_of_items > 0) {
        common_array_t *common_items = create_common_array(number_of_items);
        printf("Pozadavky:\n");
        while ((result = scanf(" %c", &operation)) == 1) {
            if ((result = execute_operation(operation, map, common_items, number_of_items) != 1)) {
                break;
            }
        }
        free_common_array(common_items);
    }
    free_hash_map(map);
    if (result == EOF) {
        return 1;
    }
    return 0;
}

int main(void) {
    if (!parseLog()) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    return 0;
}
