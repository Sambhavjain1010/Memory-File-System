#ifndef FILE_DELETE_H
#define FILE_DELETE_H

#include "file_operations.h"

void file_delete(int nof, char filenames[][256]){
    pthread_mutex_lock(&file_lock);
    for (int i = 0; i < nof; i++)
    {
        int flag = 0;
        for (int j = 0; j < file_count; j++)
        {
            if (strcmp(files[j]->filename, filenames[i]) == 0)
            {
                free(files[j]->filename);
                free(files[j]->content);
                free(files[j]);
                
                //Decrement the total number of file count
                files[j] = files[--file_count];

                // printf("Success: %s deleted successfully\n", filenames[i]);
                flag = 1;
                break;
            }
        }
        if (!flag){
            // printf("Error: %s does not exist\n", filenames[i]);
        }
    }
    
    pthread_mutex_unlock(&file_lock);
}

#endif