#include <stdio.h>
#include <stdlib.h>

int main() {
    int choice;

    while (1) {
        printf("Select mode:\n");
        printf("1. User Mode\n");
        printf("2. Benchmark Mode\n");
        printf("3. Exit\n");
        printf("Enter your choice (1, 2, or 3): ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("Running in User Mode...\n");
                system("./user_mode"); 
                break;
            case 2:
                printf("Running in Benchmark Mode...\n");
                system("./benchmark_mode"); 
                break;
            case 3:
                printf("Exiting program...\n");
                return 0;
            default:
                printf("Invalid choice. Please enter 1, 2, or 3.\n");
                break;
        }
    }
    return 0;
}
