#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct item {
    char name[100];
    int count;
} item_t;

typedef struct array {
    int size;
    int capacity;
    item_t *items;
} item_array_t;

typedef struct hash_map {
    item_array_t *items;
    int bucket_size;
} hash_map_t;

item_t *add_item(hash_map_t *map, char *name) {
    int hash = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        hash += name[i];
    }
    hash = hash % map->bucket_size;
    item_array_t bucket = map->items[hash];
    for (int i = 0; i < bucket.size; ++i) {
        if (strcmp(bucket.items[i].name, name) == 0) {
            bucket.items[i].count++;
            map->items[hash] = bucket;
            return &bucket.items[i];
        }
    }
    if (bucket.size == bucket.capacity) {
        bucket.capacity *= 2;
        bucket.items = (item_t *) realloc(bucket.items, sizeof(item_t) * bucket.capacity);
    }
    bucket.items[bucket.size].count = 1;
    strcpy(bucket.items[bucket.size].name, name);
    bucket.size++;
    map->items[hash] = bucket;
    return &bucket.items[bucket.size - 1];
}

hash_map_t *create_map(int bucket_size) {
    hash_map_t *map = (hash_map_t *) malloc(sizeof(hash_map_t));
    map->bucket_size = bucket_size;
    map->items = (item_array_t *) malloc(sizeof(item_array_t) * bucket_size);
    for (int i = 0; i < bucket_size; ++i) {
        map->items[i].size = 0;
        map->items[i].capacity = 1;
        map->items[i].items = (item_t *) malloc(sizeof(item_t));
    }
    return map;
}

void free_hash_map(hash_map_t *map) {
    for (int i = 0; i < map->bucket_size; ++i) {
        free(map->items[i].items);
    }
    free(map->items);
    free(map);
}

item_array_t *create_array(int capacity) {
    item_array_t *array = (item_array_t *) malloc(sizeof(item_array_t));
    array->size = 0;
    array->capacity = capacity;
    array->items = (item_t *) malloc(sizeof(item_t) * capacity);
    return array;
}

int add_item_to_array(item_array_t *array, item_t item) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->items = (item_t *) realloc(array->items, sizeof(item_t) * array->capacity);
    }
    array->items[array->size] = item;
    return ++(array->size);
}

item_t *get_last_item(item_array_t *array) {
    if (array->size == 0) {
        return NULL;
    }
    return &array->items[array->size - 1];
}

void swap_items(item_array_t *array, int from, int to) {
    item_t item = array->items[from];
    array->items[from] = array->items[to];
    array->items[to] = item;
}

void update_common_items(item_array_t *common_items, item_t *item, int number_of_items) {
    int from = -1, to = -1;
    for (int i = 0; i < common_items->size; ++i) {
        if (common_items->items[i].count >= item->count) {
            to = i;
        }
        if (strcmp(common_items->items[i].name, item->name) == 0) {
            common_items->items[i].count = item->count;
            from = i;
            break;
        }
    }
    if (from != -1) {
        to++;
        if (to < from) swap_items(common_items, from, to);
    } else if (common_items->size < number_of_items) {
        add_item_to_array(common_items, *item);
        return;
    } else if (get_last_item(common_items)->count == item->count) {
        add_item_to_array(common_items, *item);
        return;
    }
    if (common_items->size > number_of_items) {
        int new_size = number_of_items;
        int count = common_items->items[new_size - 1].count;
        while (new_size < common_items->size && common_items->items[new_size].count == count) {
            new_size++;
        }
        common_items->size = new_size;
    }
}

int add_log_record(hash_map_t *map, item_array_t *common_items, int number_of_items) {
    char name[100];
    char space, new_line;
    int result = scanf("%c%99s%c", &space, name, &new_line);
    if (result != 3 || space != ' ' || new_line != '\n') {
        if (result == EOF) return EOF;
        return 0;
    }
    item_t *item = add_item(map, name);
    update_common_items(common_items, item, number_of_items);
    return result;
}

void print_most_common_items(item_array_t *common_items) {
    int count = -1;
    int same_count_first = -1;
    int same_count_last = 0;
    for (int i = 0; i < common_items->size; ++i) {
        if (count != common_items->items[i].count) {
            count = common_items->items[i].count;
            int same_count = 0;
            same_count_first = same_count_last = i + 1;
            while (common_items->items[same_count_last].count == count) {
                same_count_last++;
                same_count++;
            }
            if (same_count == 0) {
                same_count_last = 0;
            }
        }
        if (same_count_last >= i + 1) {
            printf("%d.-%d.", same_count_first, same_count_last);
        } else {
            printf("%d.", i + 1);
        }
        printf(" %s, %dx\n", common_items->items[i].name, count);
    }
}

int sum_array(item_array_t *array) {
    int sum = 0;
    for (int i = 0; i < array->size; ++i) {
        sum += array->items[i].count;
    }
    return sum;
}

int execute_operation(char operation, hash_map_t *map, item_array_t *common_items, int number_of_items) {
    int result;
    switch (operation) {
        case '+':
            result = add_log_record(map, common_items, number_of_items);
            break;
        case '#':
            print_most_common_items(common_items);
        case '?':
            printf("Nejprodavanejsi zbozi: prodano %d kusu\n", sum_array(common_items));
            result = 1;
            break;
        default:
            return 0;
    }
    if (result > 1) return 1;
    return result;
}

int parseLog() {
    hash_map_t *map = create_map(100);
    int result;
    int number_of_items = 0;
    char operation;
    printf("Pocet sledovanych:\n");

    if ((result = scanf("%d", &number_of_items)) == 1 && number_of_items > 0) {
        item_array_t *common_items = create_array(number_of_items);
        printf("Pozadavky:\n");
        while ((result = scanf(" %c", &operation)) == 1) {
            if ((result = execute_operation(operation, map, common_items, number_of_items) != 1)) {
                break;
            }
        }
        free(common_items->items);
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