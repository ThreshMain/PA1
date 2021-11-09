#include <stdio.h>
#include <stdlib.h>

#define MAX_ID 99999
#define MIN_ID 0
#define MAX_ACCESS_COUNT 1000000

typedef struct node_t node_t;
struct node_t {
    int number;
    node_t *next;
};

typedef struct {
    node_t *head;
    node_t *tail;
    int count;
} linked_list_t;

void linkedListFree(linked_list_t *list) {
    node_t *tmp;
    node_t *head = list->head;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void linkedListPush(int number, linked_list_t *list) {
    list->count += 1;
    node_t *node = (node_t *) malloc(sizeof(node_t));
    node->next = NULL;
    node->number = number;
    if (list->count == 1) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
    }
}

linked_list_t *createLinkedList() {
    linked_list_t *linkedList = (linked_list_t *) malloc(sizeof(linked_list_t));
    linkedList->head = NULL;
    linkedList->tail = NULL;
    linkedList->count = 0;
    return linkedList;
}

int rangeCheck(long number, long max, long min) {
    return number >= min && number <= max;
}

int
addLogRecord(int index, int counter[], int log[], linked_list_t *positions[], int uniqueIndexes[], int *uniqueCount) {
    int id;
    int result;
    if ((result = scanf("%d", &id)) == 1) {
        if (rangeCheck(id, MAX_ID, MIN_ID)) {
            if (counter[id] == 0) {
                uniqueIndexes[*uniqueCount] = index;
                *uniqueCount += 1;
                printf("> prvni navsteva\n");
            } else {
                printf("> navsteva #%d\n", counter[id] + 1);
            }
            counter[id] += 1;
            linkedListPush(index, positions[id]);
            log[index] = id;
        } else {
            return 0;
        }
    }
    return result;
}

int
countUniqueRecords(int index, int log[], linked_list_t *positions[], const int uniqueIndexes[],
                   const int *uniqueCount) {
    int from, to;
    int result;
    if ((result = scanf("%d %d", &from, &to)) == 2) {
        if (rangeCheck(from, index - 1, 0) &&
            rangeCheck(to, index - 1, from)) {
            long sum = 0;
            int i = 0;
            while (i < *uniqueCount && uniqueIndexes[i] < from) {
                linked_list_t *position = positions[log[uniqueIndexes[i]]];
                node_t *node = position->head;
                while (node != NULL) {
                    if (node->number >= from) {
                        if (node->number <= to) {
                            sum++;
                        }
                        break;
                    }
                    node = node->next;
                }
                i++;
            }
            while (i < *uniqueCount && uniqueIndexes[i] <= to) {
                sum++;
                i++;
            }
            printf("> %ld / %d\n", sum, to - from + 1);
        } else {
            return 0;
        }
    }
    return result;
}

int
executeOperation(char operation, int *index, int counter[], int log[], linked_list_t *positions[], int uniqueIndexes[],
                 int *uniqueCount) {
    int result;
    int wantedResult;
    switch (operation) {
        case '+':
            result = addLogRecord(*index, counter, log, positions, uniqueIndexes, uniqueCount);
            *index += 1;
            if (*index > MAX_ACCESS_COUNT) return 0;
            wantedResult = 1;
            break;
        case '?':
            result = countUniqueRecords(*index, log, positions, uniqueIndexes, uniqueCount);
            wantedResult = 2;
            break;
        default:
            return 0;
    }
    if (result > 0) result = result == wantedResult;
    return result;
}


int parseLog() {
    int counter[MAX_ID - MIN_ID + 1] = {0};
    int log[MAX_ACCESS_COUNT + 1] = {0};
    linked_list_t *positions[MAX_ID - MIN_ID + 1];
    for (int i = 0; i < (MAX_ID - MIN_ID + 1); ++i) {
        linked_list_t *linkedList = createLinkedList();
        positions[i] = linkedList;
    }
    int uniqueIndexes[MAX_ID - MIN_ID + 1] = {0};
    int uniqueCount = 0;
    printf("Pozadavky:\n");
    int index = 0;
    int result;
    char operation;
    while ((result = scanf(" %c", &operation)) == 1) {
        if ((result = executeOperation(operation, &index, counter, log, positions, uniqueIndexes, &uniqueCount)) != 1) {
            break;
        }
    }
    for (int i = 0; i < (MAX_ID - MIN_ID + 1); ++i) {
        linked_list_t *list = positions[i];
        linkedListFree(list);
        free(list);
    }
    if (result == EOF) {
        return 1;
    }
    return 0;
}

int main(void){
    if(!parseLog()){
        printf("Nespravny vstup.\n");
        return 1;
    }
    return 0;
}
