#ifndef __SMCCC_H__
#define __SMCCC_H__

void arm_smccc_smc(unsigned long funciton_id, unsigned long arg0,
        unsigned long arg1, unsigned long arg2);

void arm_smccc_hvc(unsigned long funciton_id, unsigned long arg0,
        unsigned long arg1, unsigned long arg2);

#endif
