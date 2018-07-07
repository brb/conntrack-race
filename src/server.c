/* Usage: ./server <PORT> */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define LEN 512

int main(int argc, char *argv[])
{
    int s;
    struct sockaddr_in si_local, si_remote;
    socklen_t rlen = sizeof(si_remote);
    char buf[LEN];
    int len;

    memset((char *) &si_local, 0, sizeof(si_local));
    si_local.sin_family = AF_INET;
    si_local.sin_port = htons(atoi(argv[1]));
    si_local.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        exit(1);
    }
    if (bind(s, (struct sockaddr*)&si_local, sizeof(si_local)) == -1) {
        exit(2);
    }

    while(1) {
        if ((len = recvfrom(s, buf, LEN, 0, (struct sockaddr *)&si_remote, &rlen)) == -1) {
            exit(3);
        }
        printf("recv from %s:%d\n", inet_ntoa(si_remote.sin_addr), ntohs(si_remote.sin_port));
        //if (sendto(s, buf, len, 0, (struct sockaddr*)&si_remote, &rlen) == -1) {
        //  exit(4);
        //}
    }

    close(s);

    return 0;
}
