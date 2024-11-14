#ifndef FILE_CREATE_H
#define FILE_CREATE_H

#include "file_operations.h"

void file_create(int nof, char filenames[][256]){
    pthread_mutex_lock(&file_lock);
    for (int i = 0; i < nof; i++)
    {
        if (file_find(filenames[i]))
        {
            // printf("Error: File %s already exists\n", filenames[i]);
            continue;
        }
        struct memFile *newfile = (struct memFile *)malloc(sizeof(struct memFile));
        if (!newfile)
        {
            // printf("Error: Memory allocation failed\n");
            continue;
        }
        newfile->filename = strdup(filenames[i]);
        newfile->content = NULL;
        newfile->size = 0;
        newfile->created = time(NULL);
        newfile->modified = time(NULL);

        //Adding new files to the overall file array
        files[file_count++] = newfile;
        // printf("Success: %s has been created successfully\n", filenames[i]);
    }
    pthread_mutex_unlock(&file_lock);
}

#endif