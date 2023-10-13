#ifndef _EXANIC_XSK_H_
#define _EXANIC_XSK_H_

int exanic_xdp_setup_prog(struct exanic *exanic,
                                 struct bpf_prog *prog);

#endif