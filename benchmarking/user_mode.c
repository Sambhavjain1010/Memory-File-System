#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_NOF 1000

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
struct memFile* file_find(char *reqdfilename){
    for (long long i = 0; i < file_count; i++)
    {
        if (strcmp(files[i]->filename, reqdfilename) == 0)
            return files[i];
    }
    return NULL;
}

void file_create(int nof, char filenames[][256]);
void file_list(int detail);
void file_delete(int nof, char filenames[][256]);
void file_write(int nof, char filenames[][256], char contents[][256]);
void file_read(char *filename);

//Function to parse on the input command
void command_execution(char *command){
    char cmd[256];
    sscanf(command, "%s", cmd);

    if (strcmp(cmd, "create") == 0)
    {   
        //Default number of files is 1
        int nof = 1;
        char filenames[MAX_NOF][256];
        
        // temporarily storing value returned by sscanf, if 1 means number of files provided
        int temp = sscanf(command, "create -n %d", &nof);
        if (temp)
        {
            //Capturing the string containing only names of files
            char *rem = command + 11; 
            for (int i = 0; i < nof; i++)
            {
                sscanf(rem, "%s", filenames[i]);
                rem += strlen(filenames[i]) + 1;    //Moving to the next filename
            }
        }
        else{
            sscanf(command, "create %s", filenames[0]);
        }
        file_create(nof, filenames);
    }
    else if(strcmp(cmd, "write") == 0)
    {
        int nof = 1;
        char filenames[MAX_NOF][256], contents[MAX_NOF][256];
        int temp = sscanf(command, "write -n %d", &nof);

        if (temp)
        {
            //Capturing the string containing only names of files
            char *rem = command + 10; 
            for (int i = 0; i < nof; i++)
            {
                sscanf(rem, "%s \"%[^\"]\"", filenames[i], contents[i]);
                rem = strstr(rem, "\"") + strlen(contents[i]) + 3;
            }
            
        }
        else{
            sscanf(command, "write %s \"%[^\"]\"", filenames[0], contents[0]);
        }
        file_write(nof, filenames, contents);
    }
    else if(strcmp(cmd, "read") == 0)
    {
        char filename[256];
        sscanf(command, "read %s", filename);
        file_read(filename);    
    }
    else if(strcmp(cmd, "delete") == 0)
    {
        int nof = 1;
        char filenames[MAX_NOF][256];
        int temp = sscanf(command, "delete -n %d", &nof);

        if (temp)
        {
            char *rem = command + 11;
            for (int i = 0; i < nof; i++)
            {
                sscanf(rem, "%s", filenames[i]);
                rem += strlen(filenames[i]) + 1;
            }
            
        }
        else{
            sscanf(command, "delete %s", filenames[0]);
        }
        file_delete(nof, filenames);
    }
    else if(strcmp(cmd, "ls") == 0)
    {
        int detail = strstr(command, "-l") != NULL;
        file_list(detail);
    }
    else if(strcmp(cmd, "exit") == 0)
    {
        printf("memFS Stops...\n");
        exit(0);
    }
    else{
        printf("Unknown command: %s. Please enter correct command\n", cmd);
    }
    
}

//Main function
int main(){
    char command[256];
    printf("memFS Starts: \n");
    pthread_mutex_init(&file_lock, NULL);
    while (1)
    {
        printf("memFS> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;
        command_execution(command);
    }
    return 0;
}

//create file function
void file_create(int nof, char filenames[][256]){
    pthread_mutex_lock(&file_lock);
    for (int i = 0; i < nof; i++)
    {
        if (file_find(filenames[i]))
        {
            printf("Error: File %s already exists\n", filenames[i]);
            continue;
        }
        struct memFile *newfile = (struct memFile *)malloc(sizeof(struct memFile));
        if (!newfile)
        {
            printf("Error: Memory allocation failed\n");
            continue;
        }

        newfile->filename = strdup(filenames[i]);
        newfile->content = NULL;
        newfile->size = 0;
        newfile->created = time(NULL);
        newfile->modified = time(NULL);

        //Adding new files to the overall file array
        files[file_count++] = newfile;
        printf("Success: %s has been created successfully\n", filenames[i]);
    }
    pthread_mutex_unlock(&file_lock);
}

//delete file function
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

                printf("Success: %s deleted successfully\n", filenames[i]);
                flag = 1;
                break;
            }
        }
        if (!flag)
            printf("Error: %s does not exist\n", filenames[i]);
    }
    
    pthread_mutex_unlock(&file_lock);
}

//list command function
void file_list(int detail){
    pthread_mutex_lock(&file_lock);
    if (detail)
    {
        printf("Size    Created         Last Modified   FileName\n");
        for (int i = 0; i < file_count; i++)
        {
            struct tm *created_time = localtime(&files[i]->created);
            struct tm *modified_time = localtime(&files[i]->modified);
            char created_str[20], modified_str[20];
            strftime(created_str, sizeof(created_str), "%d/%m/%Y", created_time);
            strftime(modified_str, sizeof(modified_str), "%d/%m/%Y", modified_time);

            printf("%-7zu %-15s %-15s %s\n", files[i]->size, created_str, modified_str, files[i]->filename);
        }
    }
    else{
        printf("FileName: \n");
        for (int i = 0; i < file_count; i++)
        {
            printf("%s\n", files[i]->filename);
        }
    }
    pthread_mutex_unlock(&file_lock);
}

//write file function
void file_write(int nof, char filenames[][256], char contents[][256]){
    pthread_mutex_lock(&file_lock);
    for (int i = 0; i < nof; i++)
    {
        struct memFile *reqdfile = file_find(filenames[i]);
        if(!reqdfile){
            printf("Error: Couldn't find the file %s\n", filenames[i]);
            continue;
        }
        
        if (reqdfile->content)
            free(reqdfile->content);
        reqdfile->content = strdup(contents[i]);
        
        reqdfile->size = strlen(contents[i]);
        reqdfile->modified = time(NULL);

        printf("Content successfully written on %s\n", filenames[i]);
    }
    pthread_mutex_unlock(&file_lock);
}

//read file function
void file_read(char *reqdfilename){
    pthread_mutex_lock(&file_lock);
    struct memFile *reqdfile = file_find(reqdfilename);
    if (!reqdfile)
    {
        printf("Error: %s doesn't exist\n", reqdfilename);
    }else{
        if (reqdfile->content == NULL){
            printf("Content: (empty)\n");
        }else{
            printf("Content: %s\n", reqdfile->content);
        }
    }
    pthread_mutex_unlock(&file_lock);
}