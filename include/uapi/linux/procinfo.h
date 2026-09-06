#ifndef _UAPI_LINUX_PROCINFO_H
#define _UAPI_LINUX_PROCINFO_H

#include <linux/types.h>

struct procinfo {
    __u32 pid;
    __u32 ppid;
    __u32 state;
    __u32 nice;
    __u64 nvcsw;
    __u64 nivcsw;
};

#endif /* _UAPI_LINUX_PROCINFO_H */
