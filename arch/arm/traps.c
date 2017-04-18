#include <stdio.h>
#include <arch/armv7.h>
#include <core/scheduler.h>
#include <core/sched/sched-config.h>
#include <core/vm/vm.h>
#include <core/vm/vcpu.h>

#include "cp15.h"

// TODO(wonseok): If the traps cause the undefined exception or
//                abort exception, we must forward the exception to guest VM.
#define INVALID_HSR         -1

struct arm_smccc_res {
    unsigned long a0;
    unsigned long a1;
    unsigned long a2;
    unsigned long a3;
};


int do_hyp_trap(struct core_regs *regs)
{
    uint8_t pcpuid = smp_processor_id();
    int ret = INVALID_HSR;
    uint32_t hsr = read_cp32(HSR);
    uint32_t ec = EC(hsr);
    uint32_t il = IL(hsr);
    uint32_t iss = ISS(hsr);
    uint32_t vcpuid = get_current_vcpuid();

    switch (ec) {
    case EC_UNK:
        /*
         * if (HCR.TGE == 1) 
         *      handling undefined instruction
         * else 
         *      invalid_hsr;
         */
    case EC_WFI_WFE:
        /* if (CV(iss) == 1) {
         *     condition = COND(ISS); 
         * }
         */
    case EC_MCR_MRC_CP15:
        ret = emulate_cp15_32(regs, iss);
        break;
    case EC_MCRR_MRRC_CP15:
        ret = emulate_cp15_64(regs, iss);
        break;
    case EC_MCR_MRC_CP14:
    case EC_LDC_STC_CP14:
    case EC_HCRTR_CP0_CP13:
    case EC_MRC_VMRS_CP10:
    case EC_BXJ:
    case EC_MRRC_CP14:
    case EC_SVC:
    case EC_HVC:
        break;
    case EC_SMC:
    // TODO(jigi.kim): Make it as a new file or function
    {
        // TODO(jigi.kim): Will be changed into switch statement
        // TODO(jigi.kim): Give new names for r0 - r3
        if (regs->gpr[0] != 0x95c1ba60) break;

        printf("Trapped: CPU_ON\n");

        int i;
        for (i = 0; i < 13; i++)
            printf("R%d [%x]\n", i, regs->gpr[i]);

        struct vmcb *vm = get_current_vm();
        int id = regs->gpr[1];
        struct vcpu *vcpu = vm->vcpu[id];

        vcpu->regs.core_regs.pc = regs->gpr[2];
        regs->gpr[0] = 0;

//        struct arm_smccc_res *reg;
//        reg = (struct arm_smccc_res *) (* (void **) (regs->gpr[12]-4));
//        reg->a0 = 0;

        sched_vcpu_attach(vcpu->vcpuid, schedconf_g_vcpu_to_pcpu_map[vcpu->vcpuid]);
//        sched_vcpu_attach(vcpu->vcpuid, 1);
        printf("sched_vcpu_register, vcpuid : %d is attatched on pcpuid : %d\n", vcpu->vcpuid, vcpu->pcpuid);
        vcpu->state = VCPU_ACTIVATED;

        ret = 0;
        break;
    }
    case EC_PABT_FROM_GUEST:
    case EC_PABT_FROM_HYP:
        break;
    case EC_DABT_FROM_GUEST:
        ret = handle_data_abort(regs, iss);
    case EC_DABT_FROM_HYP:
        break;
    default:
        break;
    }

    if (ret == INVALID_HSR) {
        goto trap_error;
    }

    if (il == IL_ARM) {
        regs->pc += 4;
    } else {
        regs->pc += 2;
    }

    return 0;

trap_error:
    printf("CPU[%d] VCPU[%d] %s EC: 0x%x ISS: 0x%x\n", pcpuid, vcpuid, __func__, ec, iss);
    printf("r0 %x\n", regs->gpr[0]);
    printf("r1 %x\n", regs->gpr[1]);
    printf("r2 %x\n", regs->gpr[2]);
    printf("r3 %x\n", regs->gpr[3]);
    printf("guest pc is %x\n", regs->pc);
    printf("hifar %x\n", read_cp32(HIFAR));
    while (1) ;

    return INVALID_HSR;
}
