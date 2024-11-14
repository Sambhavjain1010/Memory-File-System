#ifndef FILE_LIST_H
#define FILE_LIST_H

#include "file_operations.h"

void file_list(int detail){
    pthread_mutex_lock(&file_lock);
    if (detail)
    {
        // printf("Size    Created         Last Modified   FileName\n");
        for (int i = 0; i < file_count; i++)
        {
            struct tm *created_time = localtime(&files[i]->created);
            struct tm *modified_time = localtime(&files[i]->modified);
            char created_str[20], modified_str[20];
            strftime(created_str, sizeof(created_str), "%d/%m/%Y", created_time);
            strftime(modified_str, sizeof(modified_str), "%d/%m/%Y", modified_time);

            // printf("%-7zu %-15s %-15s %s\n", files[i]->size, created_str, modified_str, files[i]->filename);
        }
    }
    else{
        // printf("FileName: \n");
        for (int i = 0; i < file_count; i++)
        {
            // printf("%s\n", files[i]->filename);
        }
    }
    pthread_mutex_unlock(&file_lock);
}

#endif