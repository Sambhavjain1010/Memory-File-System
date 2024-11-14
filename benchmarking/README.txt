24CS60R39

Task1: Building memFS which operates directly from command line. memFS is constrained only by available RAM on the system.
       This memFS supports siz commands which are as follows:
       1. create: This command can create one or more files in the memory.
                  execution: To create one file
                                create <filename>
                             To create more number of files in a single command
                                create -n <number of files> <filename>

        2. write: This command can write some content in one or more files in the memory. 
                  execution: To write in one file
                                write <filename> "Content"
                             To write in more number of files in a single command
                                create -n <number of files> <filename1> "Content1" <filename2> "Content2"

        3. delete: This command can delete one or more files in the memory.
                   execution: To delete 1 file
                                delete <filename>
                             To delete more number of files in a single command
                                delete -n <number of files> <filename> 

        4. read: This command reads content from a specified file.
                 execution: To read file
                                read <filename>
                        
        5. list: This command lists the files that currently exists in the memory.
                 execution:  ls: This will list the names of all the files currently in the memory.
                             ls -l: This will list the names along with the detail of every file in the memory.
        6. exit: Normal exit command exits the program.

Task2: Benchmarking: Running 4 commands: create, delete, write, read
                     Number of threads: {1, 2, 4, 8, 16}
                     Number of commands: {1000, 10000, 100000}