#include "kernel/types.h"
#include "kernel/stat.h"
# include "user/user.h"

# include "logger.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        LOGGER(ERROR, "only one cmd parameter is needed, got %d", argc-1);
        exit(1);
    }

    int index = 0;
    while (argv[1][index] != '\0') {
        if (argv[1][index] < '0' || argv[1][index] > '9') {
            LOGGER(ERROR, "expected a non-negtive integer, got %s", argv[1]);
            exit(1);
        }
        index += 1;
    }

    int n = atoi(argv[1]);
    if (n < 0) {
        LOGGER(ERROR, "expected a non-negtive integer, got %s", argv[1]);
        exit(1);
    }

    int retval = sleep(n);
    if (retval != 0) {
        LOGGER(ERROR, "sleep error!");
        exit(1);
    }

    exit(0);
}