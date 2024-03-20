#ifndef _BRANCH_H
#define _BRANCH_H

#include <pt_regs.h>

static inline unsigned long exception_era(struct pt_regs *regs)
{
	return regs->csr_era;
}

static inline void compute_return_era(struct pt_regs *regs)
{
	regs->csr_era += 4;
}

#endif /* _BRANCH_H */
