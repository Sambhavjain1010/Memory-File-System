#ifndef FILE_FIND_H
#define FILE_FIND_H

#include "file_operations.h"

struct memFile* file_find(char *reqdfilename){
    for (long long i = 0; i < file_count; i++)
    {
        if (strcmp(files[i]->filename, reqdfilename) == 0)
            return files[i];
    }
    return NULL;
}

#endif