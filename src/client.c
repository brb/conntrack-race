/* Usage: ./client <IP> <PORT> */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define THREAD_COUNT 10

int s;
struct sockaddr_in si_local, si_remote;
char msg[100] = "foobar";
int wait = 0;

void *worker(void *arg)
{
    while (wait != 1);

    if (sendto(s, msg, strlen(msg), MSG_NOSIGNAL, (struct sockaddr*) &si_remote, sizeof(si_remote)) == -1) {
        exit(4);
    }
}

int main(int argc, char *argv[])
{
    memset((char *) &si_local, 0, sizeof(si_local));
    memset((char *) &si_remote, 0, sizeof(si_remote));
    si_local.sin_family = AF_INET;
    si_local.sin_port = htons(0);
    si_local.sin_addr.s_addr = htonl(INADDR_ANY);
    si_remote.sin_family = AF_INET;
    si_remote.sin_port = htons(atoi(argv[2]));
    si_remote.sin_addr.s_addr = inet_addr(argv[1]);

    if ((s = socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC|SOCK_NONBLOCK, IPPROTO_UDP)) == -1) {
        exit(1);
    }
    if (bind(s, (struct sockaddr*)&si_local, sizeof(si_local)) == -1) {
        exit(2);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_t info = (pthread_t)malloc(sizeof(pthread_t));
        int err = pthread_create(&info, NULL, &worker, NULL);
        if (err != 0) {
            exit(3);
        }
    }

    wait = 1;
    usleep(10);

    close(s);

    return 0;
}
