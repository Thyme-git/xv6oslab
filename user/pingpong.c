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