#include "file_write.h"
#include "file_read.h"
#include "file_operations.h"
#include "commands_execution.h"
#include "file_list.h"
#include "file_find.h"
#include "file_delete.h"
#include "file_create.h"
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>


void commands_generation(char *command_file, int noc){
    FILE *file = fopen(command_file, "w");
    if (!file) {
        printf("Error opening the file for writing\n");
        return;
    }

    const char *commands[] = {"create", "write", "read", "delete"};
    char filenames[MAX_NOF][MAX_NAME_LEN];
    char *contents[MAX_NOF]; // Dynamically allocated strings

    srand(time(NULL));

    for (int i = 0; i < noc; i++) {
        int command_type = rand() % 4;
        int nof_in_command = (rand() % 5) + 1;

        for (int j = 0; j < nof_in_command; j++) {
            int fileno = rand() % 10;
            snprintf(filenames[j], MAX_NAME_LEN, "file%d", fileno);

            // Allocate memory for content and create string
            contents[j] = (char *)malloc(MAX_CONTENT_LEN * sizeof(char));
            if (contents[j] == NULL) {
                printf("Memory allocation failed\n");
                fclose(file);
                return;
            }
            snprintf(contents[j], MAX_CONTENT_LEN, "helloworld%d", fileno);
        }

        switch (command_type) {
            case 0: // create
                if (nof_in_command > 1) {
                    fprintf(file, "create -n %d ", nof_in_command);
                    for (int j = 0; j < nof_in_command; j++)
                        fprintf(file, "%s ", filenames[j]);
                } else {
                    fprintf(file, "create %s", filenames[0]);
                }
                fprintf(file, "\n");
                break;
            case 1: // write
                if (nof_in_command > 1) {
                    fprintf(file, "write -n %d ", nof_in_command);
                    for (int j = 0; j < nof_in_command; j++)
                        fprintf(file, "%s \"%s\" ", filenames[j], contents[j]);
                } else {
                    fprintf(file, "write %s \"%s\" ", filenames[0], contents[0]);
                }
                fprintf(file, "\n");
                break;
            case 2: // read
                fprintf(file, "read %s\n", filenames[rand() % nof_in_command]);
                break;
            case 3: // delete
                if (nof_in_command > 1) {
                    fprintf(file, "delete -n %d ", nof_in_command);
                    for (int j = 0; j < nof_in_command; j++) {
                        fprintf(file, "%s ", filenames[j]);
                    }
                } else {
                    fprintf(file, "delete %s", filenames[0]);
                }
                fprintf(file, "\n");
                break;
        }

        // Free allocated memory for contents after each command
        for (int j = 0; j < nof_in_command; j++) {
            free(contents[j]);
        }
    }

    fclose(file);
}

void file_commands_execution(const char *command_file){
    FILE *file = fopen(command_file, "r");
    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    char command[MAX_NAME_LEN];
    while(fgets(command, sizeof(command), file)){
        command_execution(command);
    }
    fclose(file);
}

// Function to execute benchmark with multiple threads
void *benchmark_thread(void *arg) {
    const char *command_file = (const char *)arg;

    // Execute commands from the file
    file_commands_execution(command_file);

    return NULL;
}

void run_benchmarking(int num_threads, char *command_file, int noc){
    pthread_t threads[num_threads];

    //Generating random commands and save to text file
    commands_generation(command_file, noc);

    //Launching the threads
    for (int i = 0; i < num_threads; i++)
    {
        pthread_create(&threads[i], NULL, benchmark_thread, (void*) command_file);
    }
    
    // Waiting for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}

//function to measure time in milliseconds
long long current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
}

//Function to measure latency
long long measure_latency(void (*workload)(int, const char *, int), int num_runs, int num_threads, const char *command_file, int num_commands) {
    long long total_latency = 0;

    for (int i = 0; i < num_runs; i++) {
        long long start_time = current_time();
        
        workload(num_threads, command_file, num_commands);

        long long end_time = current_time();
        long long latency = end_time - start_time;
        total_latency += latency;
    }

    return total_latency / num_runs; 
}

//Function to measure cpu memory usage
void cpu_memory_usage(double *cpu_time, long *memory_usage) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    *cpu_time = (usage.ru_utime.tv_sec + usage.ru_stime.tv_sec) * 1000.0 + 
                (usage.ru_utime.tv_usec + usage.ru_stime.tv_usec) / 1000.0;
    *memory_usage = usage.ru_maxrss;  
}

int main(){
    char *command_file = "benchmarking_commands.txt";
    pthread_mutex_init(&file_lock, NULL);
    char command[256];
    printf("memFS Starts: \n");
    FILE* file = fopen("benchmark_result.txt","w");
    int num_t[] = {1, 2, 4, 8, 16};
    int num_c[] = {100, 1000, 10000};
    for (int i = 0; i < 5; i++)
    {
        int not = num_t[i];
        for (int j = 0; j < 3; j++)
        {
            int noc = num_c[j];

            time_t start = current_time();

            run_benchmarking(not, command_file, noc);

            time_t end = current_time();

            double duration = (double)(end - start);
            printf("Benchmark with %d threads completed in %.2f milliseconds\n", not, duration);
            printf(file, "Benchmark with %d threads completed in %.2f milliseconds\n", not, duration);

            // Calculating the average latency and memory utilization
            long long avg_latency = measure_latency(run_benchmarking, 10, not, command_file, noc);
            double cpu_usage;
            long memory_usage;
            cpu_memory_usage(&cpu_usage, &memory_usage);

            fprintf(file,"Benchmark with %d threads and %d commands:\n", not, noc);
            fprintf(file, "Average Latency: %lld ms\n", avg_latency);
            fprintf(file, "CPU Time: %.2f ms\n", cpu_usage);
            fprintf(file, "Memory Usage: %ld KB\n", memory_usage);
            fprintf(file, "\n");
        }
        printf("\n");
    }
    pthread_mutex_destroy(&file_lock);
    return 0;
}