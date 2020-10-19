/*
    测试多线程下，SO_REUSEPORT 这个参数的作用。
    可以简单实现多个线程的并发响应，而不用使用多路复用机制。
    
    g++ -o reuserport_test reuserport_test.cpp  -pthread -Wall
*/
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <pthread.h>

void* do_work(void *arg)
{
    int *port = (int *) arg;

    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    int one = 1;
    setsockopt(listen_socket, SOL_SOCKET, SO_REUSEPORT, &one, sizeof(one));

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(*port);

    int ret = bind(listen_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)); 
    listen(listen_socket, 5);

    struct sockaddr_in cli_addr;
    memset(&cli_addr, 0, sizeof(cli_addr));
    int addr_length = sizeof(cli_addr);
    
    printf("thread %lu listen %d\n", pthread_self(), *port);
    do
    {
        int cli_sock = accept(listen_socket, (struct sockaddr *) &cli_addr, (socklen_t *) &addr_length);
        printf("In thread %lu, new connection %d is ok.\n", pthread_self(), cli_sock);
        send(cli_sock, "Hello.", 6, 0);
        sleep(10); // 延时模拟
        close(cli_sock);
    } while (true);

    close(listen_socket);

    return 0;
}

int main(int ac, const char *av[])
{ 
    int port = 3456;
    if(ac == 2)
        port = atoi(av[1]);

    const int MAX_THREADS = 10;
    pthread_t tid[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&tid[i], NULL, do_work, &port);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }
    return 0;
}
