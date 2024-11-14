#ifndef FILE_WRITE_H
#define FILE_WRITE_H

#include "file_operations.h"

void file_write(int nof, char filenames[][256], char contents[][256]){
    pthread_mutex_lock(&file_lock);
    for (int i = 0; i < nof; i++)
    {
        struct memFile *reqdfile = file_find(filenames[i]);
        if(!reqdfile){
            // printf("Error: Couldn't find the file %s\n", filenames[i]);
            continue;
        }
        
        if (reqdfile->content)
            free(reqdfile->content);
        reqdfile->content = strdup(contents[i]);
        
        reqdfile->size = strlen(contents[i]);
        reqdfile->modified = time(NULL);

        // printf("Content successfully written on %s\n", filenames[i]);
    }
    pthread_mutex_unlock(&file_lock);
}

#endif