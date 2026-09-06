#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#ifndef __NR_procinfo
#define __NR_procinfo 444
#endif

struct procinfo {
	unsigned int pid;
	unsigned int ppid;
	unsigned int state;
	unsigned int nice;
	unsigned long long nvcsw;
	unsigned long long nivcsw;
};

static const char states[] = "RSDTtXZxKWP";

static long query(int pid, struct procinfo *pi)
{
	if (syscall(__NR_procinfo, pid, pi) < 0)
		return -1;
	return 0;
}

int main(int argc, char **argv)
{
	int pid = argc > 1 ? atoi(argv[1]) : getpid();
	struct procinfo pi;

	if (query(pid, &pi) < 0) {
		printf("procinfo(%d): FAILED: errno=%d (%s)\n",
		       pid, errno, strerror(errno));
		return 1;
	}

	printf("pid=%u ppid=%u state=%c(%u) nice=%d nvcsw=%llu nivcsw=%llu\n",
	       pi.pid, pi.ppid,
	       pi.state < sizeof(states) - 1 ? states[pi.state] : '?',
	       pi.state, (int)pi.nice, pi.nvcsw, pi.nivcsw);
	return 0;
}
