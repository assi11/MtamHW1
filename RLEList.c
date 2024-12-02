#include "RLEList.h"
#include <stdlib.h>

struct RLEList_t{
    char value;
    int repeat;
    struct RLEList_t *next;
};

int findPlaces (int repeat){
    int count = 0;
    while(repeat != 0){
        count++;
        repeat = repeat / 10;
    }
    return count;
}

void mergeNodes( RLEList list ){
    RLEList copyList = list;
    RLEList temp = list;
    while (copyList -> next != NULL){
        if (copyList -> value == copyList -> next -> value){
            temp = copyList -> next ;
            copyList -> repeat += temp -> repeat;
            copyList -> next = temp -> next;
            free (temp);
        } else {
            copyList = copyList -> next;
        }
    }
}

RLEList RLEListCreate(){
    RLEList newList = malloc(sizeof (*newList));
    if(!newList){
        return NULL;
    }
    newList -> value = '\0';
    newList -> repeat = 0;
    newList -> next = NULL;
    return newList;
}

void RLEListDestroy(RLEList list){
    if (list == NULL) {
        return;
    }
    while (list -> next) {
        RLEList delete = list;
        list = list -> next;
        free(delete);
    }
    free(list);
    return;
}

RLEListResult RLEListAppend(RLEList list, char value){

    if (list == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }

    RLEList last = list;

    while (last -> next != NULL){
        last = last -> next;
    }

    if (last -> value == value){
        last -> repeat++;
        return RLE_LIST_SUCCESS;
    }

    RLEList newList = RLEListCreate();

    newList -> value = value;
    newList -> repeat = 1;
    newList -> next = NULL;
    last -> next = newList;
    return RLE_LIST_SUCCESS;
}

int RLEListSize(RLEList list){
    if(list == NULL){
        return -1;
    }
    int size = 0;

    while (list != NULL){
        size += list -> repeat;
        list = list -> next;
    }
    return size;
}

RLEListResult RLEListRemove(RLEList list, int index){
    int currentIndex = -1;
    RLEList currentNode = list;
    RLEList pastNode = list;
    if (list == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    if(index < 0 || index >= RLEListSize(list)){
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }
    while (currentIndex <=  index ){
        for(int j = 0; j < currentNode -> repeat; j++)
        {
            currentIndex ++;
            if (currentIndex == index)
            {
                if (currentNode -> repeat > 1)
                {
                    currentNode -> repeat = currentNode -> repeat - 1;
                    return RLE_LIST_SUCCESS;
                }
                if (currentNode -> repeat == 1)
                {
                    pastNode -> next = currentNode -> next;
                    currentNode -> next = NULL;
                    free(currentNode);
                    mergeNodes(list);
                    return RLE_LIST_SUCCESS;
                }
            }
        }
        pastNode = currentNode;
        currentNode = currentNode -> next ;
    }
    mergeNodes(list);
    return RLE_LIST_SUCCESS;
}

char RLEListGet(RLEList list, int index, RLEListResult *result){
    if(list == NULL){
        *result = RLE_LIST_NULL_ARGUMENT;
        return 0;
    }
    if(index < 0 || index >= RLEListSize(list)){
        *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        return 0;
    }
    while (list -> repeat <= index && list != NULL) {
        index -= list -> repeat;
        list = list -> next;
    }
    if (result) {
        *result = RLE_LIST_SUCCESS;
    }
    return list -> value;
}


RLEListResult RLEListMap(RLEList list, MapFunction map_function) {
    if (list == NULL || map_function == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    RLEList tracker = list;
    while (tracker != NULL) {
        tracker -> value = map_function(tracker -> value);
        tracker = tracker -> next;
    }
    mergeNodes(list);
    return RLE_LIST_SUCCESS;
}


int placesForRepeat (int repeat){
    int count = 0;
    while (repeat != 0) {
        repeat /= 10;
        count++;
    }
    return count;
}

char *RLEListExportToString(RLEList list, RLEListResult *result) {
    if (list == NULL) {
        if( result ){
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        return NULL;
    }
    RLEList current = list;
    if (current == NULL) {
        if(result){
            *result = RLE_LIST_OUT_OF_MEMORY;
        }
        return NULL;
    }
    if(current -> repeat == 0)
    {
        current = list -> next;
    }
    else{
        current = list;
    }
    int count = 0;

    while (current != NULL) {
        count = count+2; // the char +\n
        count = count + placesForRepeat(current -> repeat);
        current = current -> next;
    }
    count++;

    char *str =  malloc((count) * sizeof(char));
    if (str == NULL) {
        if(result){
            *result = RLE_LIST_OUT_OF_MEMORY;
        }
        return NULL;
    }

    str[count - 1] = '\0';
    if(RLEListSize(list) == 0){
        if(result){
            *result = RLE_LIST_SUCCESS;
        }
        return str;
    }
    RLEList current2 = list;
    if ( current2 -> repeat == 0)
    {
        current2 = current2 -> next;
    }

    int copy;
    int j;

    if (str == NULL) {
        if(result){
            *result = RLE_LIST_OUT_OF_MEMORY;
        }
        return NULL;
    }
    for (int i = 0; i < count - 1; ) {
        str[i] = current2 -> value;
        i++;
        j = placesForRepeat(current2 -> repeat)-1;
        copy = current2 -> repeat;
        while (j >= 0) {

            str[i + j] = (copy % 10) + '0';
            copy = copy / 10;
            j--;
        }
        i = i + placesForRepeat(current2 -> repeat);

        str [i] = '\n';
        i++;
        current2 = current2 -> next;

    }

    if(result){
        *result = RLE_LIST_SUCCESS;
    }
    return str;
}