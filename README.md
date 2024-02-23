# xv6lab for operating system course

## preparation

### logger

> in user/logger.h

```c
# ifndef LOGGER_H
# define LOGGER_H

# define DEBUG              "\033[36mDEBUG\033[0m"
# define INFO               "\033[34mINFO\033[0m"
# define WARNING            "\033[33mWARNING\033[0m"
# define ERROR              "\033[31mERROR\033[0m"
# define FATAL              "\033[31mFATAL\033[0m"
# define LOGGER(level,formatter,...)  printf("[%s:%d]-[%s] - " formatter "\n",__FILE__,__LINE__,level,## __VA_ARGS__ )
# endif
```

* add logs to your code simply using:

```c
LOGGER(INFO, "message: %d", msg);
```

## utils

### sleep

> in user/sleep.c

```c
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
```

### pingpong

> in user/pingpong.c

```c
#include "kernel/types.h"
#include "kernel/stat.h"
# include "user/user.h"

# include "logger.h"

# define ABYTE 0x29

int main(int argc, char* argv[]) {
    int pipefd[2] = {0};
    int retval = pipe(pipefd);
    if (retval != 0) {
        LOGGER(ERROR, "creating pipe error");
        exit(1);
    }

    char abyte = ABYTE;

    retval = fork();
    if (retval < 0) {
        // fork error
        LOGGER(ERROR, "fork error!");
        exit(1);
    }
    else if (retval > 0) {
        // parent process
        int pid = getpid();
        
        write(pipefd[1], &abyte, 1);
        read(pipefd[0], &abyte, 1);
        printf("%d: received pong\n", pid);
        
        close(pipefd[0]);
        close(pipefd[1]);
    }
    else if (retval == 0) {
        // child process
        int pid = getpid();
        
        read(pipefd[0], &abyte, 1);
        printf("%d: received ping\n", pid);
        write(pipefd[1], &abyte, 1);

        close(pipefd[0]);
        close(pipefd[1]);
    }
    
    exit(0);
}
```