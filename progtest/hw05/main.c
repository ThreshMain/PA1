#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct {
    char name[100];
    unsigned int hash;
    int count;
    int index_in_common_array;
} item_t;

typedef struct {
    int size;
    int capacity;
    item_t **items;
} item_array_t;

typedef struct {
    item_array_t **items;
    int bucket_size;
    int unique_count;
} hash_map_t;

typedef struct {
    int capacity;
    int total_sum;
    item_array_t **records;
} map_t;

unsigned int hash_string(char *str) {
    unsigned int hash = 149;
    char c;
    while ((c = *str++)) {
        hash = c + (hash << 6);
    }
    return hash;
}

item_t *create_item(char *name, unsigned int hash) {
    item_t *item = (item_t *) malloc(sizeof(item_t));
    strcpy(item->name, name);
    item->hash = hash;
    item->count = 1;
    item->index_in_common_array = -1;
    return item;
}


item_array_t *create_array(int capacity) {
    item_array_t *array = (item_array_t *) malloc(sizeof(item_array_t));
    array->size = 0;
    array->capacity = capacity;
    array->items = (item_t **) malloc(sizeof(item_t *) * capacity);
    return array;
}

/**
 *
 * @param array
 * @param item
 * @return index of the added item
 */
int add_item_to_array(item_array_t *array, item_t *item) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->items = (item_t **) realloc(array->items, sizeof(item_t *) * array->capacity);
    }
    array->items[array->size] = item;
    array->size++;
    return array->size - 1;
}

/**
 *
 * @param array
 * @param item
 * @return 1 if removed 0 otherwise
 */
int remove_item_from_array(item_array_t *array, int index) {
    if (index >= 0 && index < array->size) {
        array->items[index] = array->items[array->size - 1];
        array->items[index]->index_in_common_array = index;
        array->size--;
        return 1;
    }
    return 0;
}

int array_sum(item_array_t *array) {
    int sum = 0;
    for (int i = 0; i < array->size; ++i)
        sum += array->items[i]->count;
    return sum;
}

void free_array(item_array_t *array) {
    for (int i = 0; i < array->size; ++i) {
        free(array->items[i]);
    }
    free(array->items);
    free(array);
}

void resize_hash_map(hash_map_t *map) {
    if ((map->bucket_size/4) > map->unique_count) {
        return;
    }
    item_array_t **old_items = map->items;
    map->items = (item_array_t **) malloc(sizeof(item_array_t *) * map->bucket_size * 2);
    for (int i = 0; i < map->bucket_size * 2; i++) {
        map->items[i] = create_array(1);
    }
    for (int i = 0; i < map->bucket_size; ++i) {
        item_array_t *bucket = old_items[i];
        for (int j = 0; j < bucket->size; ++j) {
            item_t *item = bucket->items[j];
            int index = item->hash % (map->bucket_size * 2);
            add_item_to_array(map->items[index], item);
        }
        free(bucket->items);
        free(bucket);
    }
    map->bucket_size *= 2;
    free(old_items);
}

item_t *add_item(hash_map_t *map, char *name) {
    unsigned int hash = hash_string(name);
    unsigned int bucket_number = hash % map->bucket_size;
    item_array_t *bucket = map->items[bucket_number];
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

hash_map_t *create_hash_map(int bucket_size) {
    hash_map_t *map = (hash_map_t *) malloc(sizeof(hash_map_t));
    map->bucket_size = bucket_size;
    map->items = (item_array_t **) malloc(sizeof(item_array_t *) * bucket_size);
    for (int i = 0; i < bucket_size; ++i) {
        map->items[i] = create_array(1);
    }
    return map;
}

void free_hash_map(hash_map_t *map) {
    for (int i = 0; i < map->bucket_size; ++i) {
        free_array(map->items[i]);
    }
    free(map->items);
    free(map);
}

map_t *create_map(int capacity) {
    map_t *map = (map_t *) malloc(sizeof(map_t));
    map->capacity = capacity;
    map->total_sum = 0;
    map->records = (item_array_t **) malloc(sizeof(item_array_t *) * map->capacity);
    for (int i = 0; i < map->capacity; ++i) {
        map->records[i] = create_array(1);
    }
    return map;
}

void resize_map(map_t *map, int minimum_capacity) {
    int new_capacity = map->capacity * 2;
    while (new_capacity < minimum_capacity) {
        new_capacity *= 2;
    }
    map->records = (item_array_t **) realloc(map->records, sizeof(item_array_t *) * new_capacity);
    for (int i = map->capacity; i < new_capacity; ++i) {
        map->records[i] = create_array(1);
    }
    map->capacity = new_capacity;
}

void update_common_items(map_t *map, item_t *item, int number_of_items) {
    if (map->capacity < item->count) {
        resize_map(map, item->count);
    }
    if (item->count > 1) {
        if (remove_item_from_array(map->records[item->count - 2], item->index_in_common_array))
            map->total_sum -= item->count - 1;
    }
    item->index_in_common_array = add_item_to_array(map->records[item->count - 1], item);
    map->total_sum += item->count;
    int sum = 0;
    for (int i = map->capacity - 1; i >= 0; --i) {
        if (sum < number_of_items) {
            sum += map->records[i]->size;
        } else {
            map->total_sum -= array_sum(map->records[i]);
            map->records[i]->size = 0;
        }
    }
}

int add_log_record(hash_map_t *map, map_t *common_items, int number_of_items) {
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

void print_most_common_items(map_t *common_items) {
    int current_position = 1;
    for (int i = common_items->capacity - 1; i >= 0; --i) {
        item_array_t *array = common_items->records[i];
        if (array->size != 0) {
            for (int j = 0; j < array->size; ++j) {
                printf("%d.", current_position);
                if (array->size > 1) {
                    printf("-%d. %s, %dx\n", current_position + array->size - 1, array->items[j]->name,
                           array->items[j]->count);
                } else {
                    printf(" %s, %dx\n", array->items[j]->name, array->items[j]->count);
                }
            }
            current_position += array->size;
        }
    }
}

int execute_operation(char operation, hash_map_t *map, map_t *common_items, int number_of_items) {
    int result;
    switch (operation) {
        case '+':
            result = add_log_record(map, common_items, number_of_items);
            break;
        case '#':
            print_most_common_items(common_items);
        case '?':
            printf("Nejprodavanejsi zbozi: prodano %d kusu\n", common_items->total_sum);
            result = 1;
            break;
        default:
            return 0;
    }
    if (result > 1) return 1;
    return result;
}

int parseLog() {
    hash_map_t *map = create_hash_map(128);
    int result;
    int number_of_items = 0;
    char operation;
    printf("Pocet sledovanych:\n");

    if ((result = scanf("%d", &number_of_items)) == 1 && number_of_items > 0) {
        map_t *common_items = create_map(number_of_items);
        printf("Pozadavky:\n");
        while ((result = scanf(" %c", &operation)) == 1) {
            if ((result = execute_operation(operation, map, common_items, number_of_items) != 1)) {
                break;
            }
        }
        for (int i = 0; i < common_items->capacity; ++i) {
            free(common_items->records[i]->items);
            free(common_items->records[i]);
        }
        free(common_items->records);
        free(common_items);
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