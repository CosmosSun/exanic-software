#include <linux/pci.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/netdevice.h>
#include <linux/miscdevice.h>
#include <linux/bpf_trace.h>
#include <linux/bpf.h>
#include <linux/filter.h>

#include "exanic-structs.h"
#include "exanic-xsk.h"

int exanic_xdp_setup_prog(struct exanic *exanic,
                                 struct bpf_prog *prog) 
{
    struct bpf_prog *old_prog;

    old_prog = xchg(&exanic->xdp_prog, prog);

    if (old_prog)
        bpf_prog_put(old_prog);
    
    return 0;
}
