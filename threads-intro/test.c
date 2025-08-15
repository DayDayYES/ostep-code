#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void *thread_function(void *arg){
    int *a = (int *) arg;
    printf("Hello from thread %d\n", *a);
    int *b = (int *) malloc(sizeof(int));
    *b = 10;
    return (void *) b;
}

int main(){
    pthread_t tid;
    int a = 5;
    pthread_create(&tid, NULL, thread_function, (void *)&a);
    pthread_join(tid, (void **)&a);
    printf("a = %d\n", a);
    return 0;
}