#include<stdio.h>
#include<stdlib.h>

int main(int ac, int **av)
{
    int exit_status = EXIT_SUCCESS;
    FILE *input;

    while (*++av != NULL) {
        input = fopen(*av, "r");
        if (input == NULL) {
            perror(*av);
            exit_status = EXIT_FAILURE;
            continue;
        }

           /* handle fi */

        if (fclose(input) != 0) {
            perror("fclose");
            exit(EXIT_SUCCESS);
        }
    }
    return exit_status;
 }