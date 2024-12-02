#include "AsciiArtTool.h"
#include <stdlib.h>
#include "RLEList.h"


RLEList asciiArtRead(FILE* in_stream) {
    RLEList list = RLEListCreate();
    if(in_stream == NULL){
        return NULL;
    }
    if(list == NULL){
        return NULL;
    }
    char value;
    while((value = fgetc(in_stream) != EOF)){
        RLEListAppend(list, value);
    }
    return list;
}



RLEListResult asciiArtPrint(RLEList list, FILE *out_stream){
    RLEListResult result;
    if(list == NULL || out_stream == NULL){
        result = RLE_LIST_NULL_ARGUMENT;
        return result;
    }

    char* temp = RLEListExportToString(list, &result);
    if (result != RLE_LIST_SUCCESS) {
        free(temp);
        return result;
    }

    fputs(temp, out_stream);
    free(temp);
    result = RLE_LIST_SUCCESS;
    return result;
}


RLEListResult asciiArtPrintEncoded(RLEList list, FILE *out_stream) {
    RLEListResult result;
    if (list == NULL || out_stream == NULL) {
        result = RLE_LIST_NULL_ARGUMENT;
        return result;
    }

    char *temp = RLEListExportToString(list, &result);

    if (result != RLE_LIST_SUCCESS) {
        return result;
    }
    fputs(temp, out_stream);
    free(temp);
    return RLE_LIST_SUCCESS;
}