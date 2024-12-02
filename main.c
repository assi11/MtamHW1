#include <stdio.h>
#include <string.h>
#include "AsciiArtTool.h"

char swapCharacter(char character) {
    if (character == ' ') {
        return '@';
    }
    if (character == '@') {
        return ' ';
    }
    return character;
}

void printErrorAndCloseFiles(FILE* source, FILE* target) {
    printf("Error\n");
    fclose(source);
    fclose(target);
}

int main(int argc, char** argv) {
    if (argc == 4) {
        char* flag = argv[1];
        FILE* source = fopen(argv[2], "r");

        if (!source) {
            printf("Error opening the source file\n");
            return 1;
        }

        FILE* target = fopen(argv[3], "w");

        if (!target) {
            printf("Error opening the target file\n");
            fclose(source);
            return 1;
        }

        RLEList list = asciiArtRead(source);

        if (strcmp(flag, "-e") == 0) {
            RLEListResult result = asciiArtPrintEncoded(list, target);

            if (result != RLE_LIST_SUCCESS) {
                printErrorAndCloseFiles(source, target);
                return 1;
            }
        } else if (strcmp(flag, "-i") == 0) {
            RLEListResult result = RLEListMap(list, swapCharacter);

            if (result != RLE_LIST_SUCCESS) {
                printErrorAndCloseFiles(source, target);
                return 1;
            }

            result = asciiArtPrint(list, target);

            if (result != RLE_LIST_SUCCESS) {
                printErrorAndCloseFiles(source, target);
                return 1;
            }
        } else {
            printf("Invalid flag. Use either -e or -i\n");
            fclose(source);
            fclose(target);
            return 1;
        }

        RLEListDestroy(list);
        fclose(source);
        fclose(target);
    } else {
        printf("./AsciiArtTool <flags> <source> <target>\n");
        return 1;
    }

    return 0;
}
