#include "Util.h"
#include <stdlib.h>

FILE* openFile(char* fileName,const char* mode)
{
    FILE* fp;

    fopen_s(&fp,fileName, mode);
    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(-1);
    }
    return fp;
}