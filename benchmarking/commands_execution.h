#ifndef COMMANDS_EXECUTION_H
#define COMMANDS_EXECUTION_H

#include "file_operations.h"

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
        if (temp == 1) {
            char *rem = command + 10;
            for (int i = 0; i < nof; i++) {
                sscanf(rem, "%s \"%[^\"]\"", filenames[i], contents[i]);
                rem = strstr(rem, "\"") + strlen(contents[i]) + 3; // Adjust for closing quote and space
            }
        } else {
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

#endif