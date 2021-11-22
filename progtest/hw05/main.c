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
    int size;
    int capacity;
    int *data;
} int_array_t;

typedef struct {
    item_array_t *records;
    int_array_t *indexes;
} common_array_t;

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


item_array_t *create_item_array(int capacity) {
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

void add_int_to_array(int_array_t *array, int value) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->data = (int *) realloc(array->data, sizeof(int) * array->capacity);
    }
    array->data[array->size] = value;
    array->size++;
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

void resize_hash_map(hash_map_t *map) {
    if ((map->bucket_size / 4) > map->unique_count) {
        return;
    }
    item_array_t **old_items = map->items;
    map->items = (item_array_t **) malloc(sizeof(item_array_t *) * map->bucket_size * 2);
    for (int i = 0; i < map->bucket_size * 2; i++) {
        map->items[i] = create_item_array(1);
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
        map->items[i] = create_item_array(1);
    }
    return map;
}

void free_hash_map(hash_map_t *map) {
    for (int i = 0; i < map->bucket_size; ++i) {
        free_item_array(map->items[i]);
    }
    free(map->items);
    free(map);
}

common_array_t *create_common_array(int capacity) {
    common_array_t *array = (common_array_t *) malloc(sizeof(common_array_t));
    array->records = create_item_array(capacity);
    array->indexes = (int_array_t *) malloc(sizeof(int_array_t));
    array->indexes->size = 0;
    array->indexes->capacity = capacity;
    array->indexes->data = (int *) malloc(sizeof(int) * capacity);
    return array;
}

void swap_items(item_array_t *array, int from, int to) {
    array->items[from]->index_in_common_array = to;
    array->items[to]->index_in_common_array = from;
    item_t *temp = array->items[from];
    array->items[from] = array->items[to];
    array->items[to] = temp;
}

int get_index_from_common_array(common_array_t *map, int index) {
    while (map->indexes->size <= index) {
        add_int_to_array(map->indexes, 0);
    }
    return map->indexes->data[index];
}

void update_common_items(common_array_t *map, item_t *item, int number_of_items) {
    if (map->records->size == 0) {
        item->index_in_common_array = map->records->size;
        add_item_to_array(map->records, item);
        add_int_to_array(map->indexes, map->records->size - 1);
        return;
    }

    if (item->index_in_common_array == -1) {
        int last_index = get_index_from_common_array(map, map->indexes->size - 1);
        if (last_index < number_of_items ||
            map->records->items[last_index]->count == item->count) {
            item->index_in_common_array = map->records->size;
            add_item_to_array(map->records, item);
        }
        return;
    }
    int from = item->index_in_common_array;
    int to = get_index_from_common_array(map, item->count - 2);
    if (from != to) {
        swap_items(map->records, from, to);
    }
    get_index_from_common_array(map, item->count - 1);
    int previous_count=item->count-2;
    map->indexes->data[previous_count]++;
}

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

void print_most_common_items(common_array_t *map, int number_of_items) {
    int position = 1;
    for (int i = map->indexes->size - 1; i >= 0; i--) {
        int index = map->indexes->data[i];
        int next_index = i >= 1 ? map->indexes->data[i - 1] : map->records->size;
        int count = next_index - index;
        if(index>=number_of_items) break;
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

int common_items_sum(common_array_t *map,int number_of_items){
    int sum=0;
    for (int i = map->indexes->size - 1; i >= 0; i--) {
        int index = map->indexes->data[i];
        int next_index = i >= 1 ? map->indexes->data[i - 1] : map->records->size;
        int count = next_index - index;
        if(index>=number_of_items) break;
        sum+=(i+1)*count;
    }
    return sum;
}

int execute_operation(char operation, hash_map_t *map, common_array_t *common_items, int number_of_items) {
    int result;
    switch (operation) {
        case '+':
            result = add_log_record(map, common_items, number_of_items);
            break;
        case '#':
            print_most_common_items(common_items,number_of_items);
        case '?':
            printf("Nejprodavanejsi zbozi: prodano %d kusu\n", common_items_sum(common_items,number_of_items));
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
        common_array_t *common_items = create_common_array(number_of_items);
        printf("Pozadavky:\n");
        while ((result = scanf(" %c", &operation)) == 1) {
            if ((result = execute_operation(operation, map, common_items, number_of_items) != 1)) {
                break;
            }
        }
        free_int_array(common_items->indexes);
        free(common_items->records->items);
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
