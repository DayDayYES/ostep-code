#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>  // 包含 open 函数和文件控制常量
#include <sys/stat.h> // 包含文件权限常量（如 S_IRWXU）
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv){
    int pipefd[2];
    pid_t pid1, pid2;

    if(pipe(pipefd) == -1){
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }

    pid1 = fork();
    if(pid1 < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if(pid1 == 0){
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        printf("child1: hello\n");
        exit(0);
    }

    pid2 = fork();
    if(pid2 < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if(pid2 == 0){
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        char buffer[100];
        int n = read(STDIN_FILENO, buffer, sizeof(buffer));
        if(n > 0){
            buffer[n] = '\0';
            printf("child2: received: %s\n", buffer);
        }
        exit(0);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}

