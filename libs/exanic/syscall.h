#ifndef __CI_UL_SYSCALL_UNIX_H__
#define __CI_UL_SYSCALL_UNIX_H__

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>

/*! Generate declarations of pointers to the system calls */
#define EXANIC_MK_DECL(ret,fn,args)  extern ret (*exanic_sys_##fn) args
# include "syscall.h.tmpl"

#endif