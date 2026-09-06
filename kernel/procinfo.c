
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/errno.h>
#include <uapi/linux/procinfo.h>

static int do_find_task(pid_t pid, struct procinfo* info)
{
    struct task_struct *p;
    int retval = 0;

    rcu_read_lock();
    p = find_task_by_vpid(pid);
    if (!p) {
        retval = -ESRCH;
        goto out;
    }

    info->pid = pid;
    info->ppid = task_ppid_nr(p);
    info->state = task_state_index(p);
    info->nice = task_nice(p);
    info->nvcsw = READ_ONCE(p->nvcsw);
    info->nivcsw = READ_ONCE(p->nivcsw);
out:
    rcu_read_unlock();
    return retval;
}


SYSCALL_DEFINE2(procinfo, pid_t, pid, struct procinfo __user*, upi)
{
    struct procinfo tmp;
    int retval = do_find_task(pid, &tmp);

    if (retval == 0) {
        if (copy_to_user(upi, &tmp, sizeof(tmp))) {
            return -EFAULT;
        }
    }

    return retval;
}

