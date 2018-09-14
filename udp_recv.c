#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/msg.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>

#define TIMING_FUNC_EX(func,s,e)            \
    ({ struct timeval s_, e_;               \
       gettimeofday(&s_, NULL);             \
       func;                                \
       gettimeofday(&e_, NULL);             \
       s = s_.tv_sec+s_.tv_usec/1000000.0;  \
       e = e_.tv_sec+e_.tv_usec/1000000.0;  \
       e - s;                               \
    })

#define START_TIMING()                      \
    ({ struct timeval s_;                   \
       gettimeofday(&s_, NULL);             \
       s_.tv_sec+s_.tv_usec/1000000.0;      \
    })

#define END_TIMING()                        \
    ({ struct timeval e_;                   \
       gettimeofday(&e_, NULL);             \
       e_.tv_sec+e_.tv_usec/1000000.0;      \
    })



uint64_t npkt = 0;
uint64_t nbyte = 0;
uint64_t nxmiterr = 0;
double   start, end, elapse = 0.0;

void sigint_handler(int sig) {
	char buf[256];
    end = END_TIMING();
    elapse = end - start;
	sprintf(buf, 
            "pkt recv'ed: %" PRIu64 "\n"
            "byte recv'ed: %" PRIu64 "\n"
            "err recv'ed: %" PRIu64 "\n"
            "recv bw (mbit/s): %lf\n",
            npkt, nbyte, nxmiterr, nbyte*8/elapse/1000000);
	write(1, buf, strlen(buf)); 
	signal(SIGINT, SIG_DFL);
	kill(getpid(), SIGINT);
}

int main(void) {
	int sockfd;
	struct sockaddr_in sa;
	int sa_len;
	char buf[65536];
	int ret;

	signal(SIGINT, sigint_handler);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
/*
	{
		int i;
		i = 4 << 16; setsockopt(sockfd,SOL_SOCKET, SO_RCVBUF, (char *)&i,sizeof(i));
		i = 4 << 16; setsockopt(sockfd,SOL_SOCKET, SO_SNDBUF, (char *)&i,sizeof(i));
	}
*/
	memset(&sa, 0, sizeof sa); 
	sa.sin_family = AF_INET;
	sa.sin_port = htons(39999);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr *)&sa, sizeof(struct sockaddr)) == -1) {
		perror("Server-bind() error !");
		return -1;
	}
    start = START_TIMING();
	while (1) {
		sa_len = sizeof sa;

		ret = recvfrom(sockfd, buf, sizeof buf, 0, (struct sockaddr *)&sa, &sa_len);
		if (ret == -1) {
            nxmiterr ++;
            continue;
        }
		npkt++;
		nbyte += ret;
	}
	return 0;
}
