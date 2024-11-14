#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_NOF 1000
#define MAX_CONTENT_LEN 1000
#define MAX_NAME_LEN 256

struct memFile{
    char *filename;
    char *content;
    size_t size;
    time_t created;
    time_t modified;
};

//Declaring a seperate structure for every file
struct memFile *files[MAX_NOF];
long long file_count = 0;
pthread_mutex_t file_lock;

//function to search a file
struct memFile* file_find(char *reqdfilename);

void file_create(int nof, char filenames[][256]);
void file_list(int detail);
void file_delete(int nof, char filenames[][256]);
void file_write(int nof, char filenames[][256], char contents[][256]);
void file_read(char *filename);

#endif