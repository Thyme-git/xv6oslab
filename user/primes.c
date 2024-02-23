#include "kernel/types.h"
#include "kernel/stat.h"
# include "user/user.h"

# include "logger.h"

# define MAX_COUNT_NUM 35

int main(int argc, char* argv[]) {
    int upstream[2] = {0};
    int retval = pipe(upstream);
    if (retval != 0) {
        LOGGER(ERROR, "creating pipe error");
        exit(1);
    }
    
    retval = fork();
    if (retval < 0) {
        LOGGER(ERROR, "fork error!");
        exit(1);
    }
    else if (retval == 0) {
        // parent process
        
        // send each number to the first child
        for (int number = 2; number <= MAX_COUNT_NUM; ++number) {
            write(upstream[1], &number, sizeof(number));
        }
        // mark ending;
        int number = -1;
        write(upstream[1], &number, sizeof(number));

        close(upstream[0]);
        close(upstream[1]);
    }
    else if (retval > 0) {
        // child process

        // read the first number as base
        int base = -1;
        while (read(upstream[0], &base, sizeof(base)), base != -1) {
            // print the first received number
            printf("prime %d\n", base);

            int downstream[2] = {0};
            retval = pipe(downstream);
            if (retval != 0) {
                LOGGER(ERROR, "creating pipe error!");
                exit(1);
            }

            retval = fork();
            if (retval < 0) {
                LOGGER(ERROR, "fork error!");
                exit(1);
            }
            else if (retval == 0) {
                // current process
                int number = -1;
                while(read(upstream[0], &number, sizeof(number)) > 0) {
                    if (number % base != 0){
                        write(downstream[1], &number, sizeof(number));
                    }
                }
                // mark ending;
                number = -1;
                write(downstream[1], &number, sizeof(number));

                close(downstream[0]);
                close(downstream[1]);
                close(upstream[0]);
                close(upstream[1]);
                exit(0);
            }
            else if (retval > 0) {
                // the next child
                close(upstream[0]);
                close(upstream[1]);

                upstream[0] = downstream[0];
                upstream[1] = downstream[1];
            }
        }

        close(upstream[0]);
        close(upstream[1]);
    }
    exit(0);
}