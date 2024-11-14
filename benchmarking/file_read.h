#ifndef FILE_READ_H
#define FILE_READ_H

#include "file_operations.h"

void file_read(char *reqdfilename){
    pthread_mutex_lock(&file_lock);
    struct memFile *reqdfile = file_find(reqdfilename);
    if (!reqdfile)
    {
        // printf("Error: %s doesn't exist\n", reqdfilename);
    }else{
        if (reqdfile->content == NULL){
            // printf("Content: (empty)\n");
        }else{
            // printf("Content: %s\n", reqdfile->content);
        }
    }
    pthread_mutex_unlock(&file_lock);
}

#endif