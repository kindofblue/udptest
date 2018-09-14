#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/msg.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <inttypes.h>
#include <unistd.h>



unsigned int ntick,nbyte;

uint64_t npkt = 0;

void sigint_handler(int sig) {
	char buf[64];
	sprintf(buf, "pkt sent: %" PRIu64 "\n", npkt);
	write(1, buf, strlen(buf)); 
	signal(SIGINT, SIG_DFL);
	kill(getpid(), SIGINT);
}


int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in sa;
    int sa_len;
    int pkt_size;
    char buf[65536];
    int ret;

    if (argc != 3) {
        printf("usage: ./udp_send <dest ip> <pkt size>\n");
        exit(-1);
    }

    signal(SIGINT, sigint_handler);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("create socket error");
        return -1;
    }
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(39999);
    sa.sin_addr.s_addr = inet_addr(argv[1]);
    sa_len = sizeof sa;

    pkt_size = atoi(argv[2]);
    while (1) {
        *(uint64_t*)buf = npkt;
        npkt++;
        ret =sendto(sockfd, buf, pkt_size, MSG_NOSIGNAL, (struct sockaddr *)&sa, sa_len);
        if (ret == -1) {
            perror("error sendto:");
        }
 //       usleep(10);
    }

    return 0;
}
