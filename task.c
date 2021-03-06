#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sched.h>

typedef struct mytime{
	unsigned long sec, nsec;
} Time;

int main(int argc, char *argv[]){
	struct sched_param param;
	int _fd = shm_open("exe_T", O_RDWR, S_IRWXU);
	if(_fd == -1){
		puts("shm GG");
		return 0;
	}
	int *_T = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0);
	int total = atoi(argv[2]);

	Time st, ed;
#ifndef NO_SYS_CALL
	if(syscall(334, &st.sec, &st.nsec) == -1) puts("child start time GG");
#endif
	while(total > 0){
		total -= *_T;
		// printf("[%s] %d is running for %d unit of time\n", argv[1], getpid(), *_T);
		// fflush(stdout);
		for(int t = 0; t < *_T; t ++){
			volatile unsigned long i;
			for (i = 0; i < 1000000UL; i ++);
		}
#ifndef NO_SYS_CALL
		if(total == 0){			
			if(syscall(334, &ed.sec, &ed.nsec) == -1) puts("child end time GG");
			char s[205];
			sprintf(s, "[Project1] %d %lu.%09lu, %lu.%09lu", getpid(), st.sec, st.nsec, ed.sec, ed.nsec);
			// printf("%s\n", s);
			if(syscall(333, s) == -1) puts("dmesg GG");
		}
#endif
        param.sched_priority = 1;
		if(sched_setparam(0, &param) < 0) puts("child: param GG");
		sched_yield();
	}
}
