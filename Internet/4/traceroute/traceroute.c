#include "traceroute.h"


int sport;
int sendfd;
int recvfd;
int ttl_max = 30;
int datalen = 60;
char sendbuf[BUFSIZE];
char recvbuf[BUFSIZE];
struct addrinfo *addr_info;
sigjmp_buf env;


void signal_handler(int signum){
	siglongjmp(env, 1);
}


int main(int argc, char *argv[]){
	
	sport = (getpid()&0xffff)|0x8000;

	if((sendfd = socket(AF_INET, SOCK_DGRAM , 0)) < 0){
		perror("socket, sock_dgram");
		return 1;
	}
	if((recvfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0){
		perror("socket, sock_raw");
		return 1;
	}
	setuid(getuid());
	

	if(trace_info(argc,argv) < 0){
		printf("trace_info\n");
		return 1;
	}

	uint16_t  nport = 35461;//port number
	struct sockaddr_in *addr_send = calloc(1, addr_info->ai_addrlen);
	struct sockaddr_in *addr_recv = calloc(1, addr_info->ai_addrlen);
	struct sockaddr_in *addr_last = calloc(1, addr_info->ai_addrlen);
	struct sockaddr_in *addr_bind = calloc(1, addr_info->ai_addrlen);
	if(memcpy(addr_send, addr_info->ai_addr, addr_info->ai_addrlen) == NULL){
		perror("memset");
		return 1;
	}
	

	addr_bind->sin_family = addr_info->ai_family ;
	addr_bind->sin_port   = htons(sport);
	if(bind(sendfd, (struct sockaddr *)addr_bind, addr_info->ai_addrlen) < 0){
		perror("bind");
		return 1;
	} 

	struct sigaction act;
	act.sa_handler = signal_handler;
	sigset_t alrm_sig;
	sigfillset(&alrm_sig);
	sigdelset(&alrm_sig, SIGALRM);
	act.sa_mask = alrm_sig;
	sigaction(SIGALRM, &act, NULL);

	int ttl;
	int done = 0;
	for(ttl = 1; ttl <= 30 && !done; ttl++){
		done == 0;
		if(setsockopt(sendfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0){
			perror("sportetsockopt");
			return 1;
		}
		printf("%d ", ttl);
		fflush(stdout);
		uint16_t nprobe = 0;
		for(nprobe = 0; nprobe < 3; nprobe ++){

			if(sigsetjmp(env, 1) != 0){
				printf(" * ");
				fflush(stdout);
				continue;
			}
			addr_send->sin_family = addr_info->ai_family;
			addr_send->sin_port = htons(nport + nprobe);
			memset(sendbuf, 0x50, datalen);
			struct timeval tv_send;
			struct timeval tv_recv;

			if(gettimeofday(&tv_send, NULL) < 0){
				perror("gettimeofday");
				return 1;
			}
	
			alarm(3);
			if(sendto(sendfd, sendbuf, datalen, 0, (struct sockaddr *)addr_send , addr_info->ai_addrlen) < 0){
					perror("sendto");
					return 1;
			}
			socklen_t recv_len = addr_info->ai_addrlen;
			if(memset(addr_recv, 0, addr_info->ai_addrlen) == NULL){
					perror("memset");
					return 1;
			}
			for(;;){
				if(recvfrom(recvfd, recvbuf, sizeof(recvbuf),  0, (struct sockaddr *)addr_recv , &recv_len) < 0){
						perror("recvfrom");
						return 1;
				}

				if(memcmp((const char *)addr_last, (const char *)addr_recv, recv_len) != 0){
						memcpy((char *)addr_last,(const char *) addr_recv, recv_len);
						char name_buf[100];
						memset(name_buf,0,100);
						if(inet_ntop(AF_INET, &addr_last->sin_addr, name_buf, sizeof(name_buf)) == NULL){
							perror("int_ntop");
							return 1;
						}
						struct addrinfo *ret;
						if(getaddrinfo(name_buf, NULL, NULL, &ret) < 0){
							perror("getaddrinfo");
							return 1;
						}
						printf("%-15s(%-15s) ", ret->ai_canonname?ret->ai_canonname:name_buf, name_buf);
						fflush(stdout);
				}

				int paser_rest ;
				paser_rest= package_paser(recvbuf);

				if( paser_rest== -3){
						done = 1;
						break;
				}

				if( paser_rest == -2){
						continue;
				}

				if(paser_rest == -1){
						break;
				}
				
			}
			alarm(0);
			char name_buf[200];
			memset(name_buf, 0, sizeof(name_buf));
			if(gettimeofday(&tv_recv, 0) < 0){
					perror("gettimeofday");
					return 1;
			}
			double time_used;
			if((tv_recv.tv_usec -= tv_send.tv_usec) < 0){
					tv_recv.tv_usec += 1000000;
			}
			time_used = (tv_recv.tv_sec - tv_send.tv_sec)*1000 + tv_recv.tv_usec/1000.0;
			printf(" %7.2fms ", time_used);
		}
		printf("\n");
	}
	
	return 0;
}




int trace_info(int argc,  char *argv[]){
	 if(argc < 2 ){
		printf("no enough parameters\n");
		return -1;
	}
	if(getaddrinfo(argv[1], NULL, NULL, &addr_info) != 0){
		printf("cannot getaddrinfo\n");
		return -1;
	}
	char addr_name[50];    
	if(inet_ntop(AF_INET, &((struct sockaddr_in *)(addr_info->ai_addr))->sin_addr, addr_name, sizeof(addr_name)) == NULL){
		perror("inet_ntop");
		return -1;
	}
	printf("traceroute to %s (%s), %d hops max, %d byte packets\n",\
										addr_info->ai_canonname?addr_info->ai_canonname:argv[1], addr_name, ttl_max, datalen);
	return  0;
}

int package_paser(char *recvbuf){
	struct ip *ip_pkg;
	int hlen1, hlen2;
	struct icmp *icmp_pkg;
	int icmplen;
	struct udphdr *udp_pkg;
	ip_pkg = (struct ip *)recvbuf;
	hlen1 = (ip_pkg->ip_hl)<<2;
	
	icmp_pkg = (struct icmp *)(recvbuf + hlen1);
	if((icmplen = ip_pkg->ip_len - hlen1) < 8){
			return -2;       // not enough to look at the icmp header
	}
	if((icmp_pkg->icmp_type == ICMP_TIMXCEED)  && (icmp_pkg->icmp_code == ICMP_TIMXCEED_INTRANS)){
			if(icmplen < 8 + sizeof(struct ip)){//icmp长度为8
					return -2;
			}
			ip_pkg = (struct ip *)(recvbuf +hlen1 + 8);//icmp头部长度为8
			hlen2 = ip_pkg->ip_hl<<2;
			if(icmplen < 8+ hlen2 + 4){
					return -2; //not engough data to look at upd data
			}
			udp_pkg = (struct udphdr *)(recvbuf + hlen1 + 8 +hlen2);
			if(ip_pkg->ip_p == IPPROTO_UDP && udp_pkg->source == htons(sport)){
					return -1;
			}
	}
	else if(icmp_pkg->icmp_type == ICMP_UNREACH){
			if(icmplen < 8 + sizeof(struct ip)){//icmp长度为8
					return -2;
			}
			ip_pkg = (struct ip *)(recvbuf +hlen1 + 8);//icmp头部长度为8
			hlen2 = ip_pkg->ip_hl<<2;
			if(icmplen < 8+ hlen2 + 4){
					return -2; //not engough data to look at upd data
			}
			udp_pkg = (struct udphdr *)(recvbuf + hlen1 + 8 +hlen2);
			if(ip_pkg->ip_p == IPPROTO_UDP && udp_pkg->source == htons(sport)){
					return -3;
			}
	}
}