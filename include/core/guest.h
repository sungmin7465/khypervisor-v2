#ifndef __GUEST_H__
#define __GUEST_H__

#include <hvmm_types.h>
#include <guest_hw.h>

#define NUM_GUEST_CONTEXTS        NUM_GUESTS_CPU0_STATIC

enum hyp_hvc_result {
    HYP_RESULT_ERET = 0,
    HYP_RESULT_STAY = 1
};

#define GUEST_VERBOSE_ALL       0xFF
#define GUEST_VERBOSE_LEVEL_0   0x01
#define GUEST_VERBOSE_LEVEL_1   0x02
#define GUEST_VERBOSE_LEVEL_2   0x04
#define GUEST_VERBOSE_LEVEL_3   0x08
#define GUEST_VERBOSE_LEVEL_4   0x10
#define GUEST_VERBOSE_LEVEL_5   0x20
#define GUEST_VERBOSE_LEVEL_6   0x40
#define GUEST_VERBOSE_LEVEL_7   0x80

// guest_struct's features will be vcpu's and change guest_struct
// this time, guest_struct is vcpu.
struct vcpu {
    struct vcpu_regs vcpu_regs;
    vmid_t vmid;
};

extern uint32_t _guest0_bin_start;
extern uint32_t _guest0_bin_end;
extern uint32_t _guest1_bin_start;
#ifdef _SMP_
extern uint32_t _guest2_bin_start;
extern uint32_t _guest2_bin_end;
extern uint32_t _guest3_bin_start;
extern uint32_t _guest3_bin_end;
#endif
//struct vcpu *_current_guest[NUM_CPUS];
struct vcpu guests[NUM_GUESTS_STATIC];

void guest_copy(struct vcpu *dst, vmid_t vmid_src);
void guest_dump_regs(struct arch_regs *regs);
hvmm_status_t guest_init();
struct vcpu get_guest(uint32_t guest_num);
void reboot_guest(vmid_t vmid, uint32_t pc, struct arch_regs **regs);

hvmm_status_t guest_save(struct vcpu *guest, struct arch_regs *regs);
hvmm_status_t guest_restore(struct vcpu *guest, struct arch_regs *regs);

static inline unsigned long num_of_guest(int cpu)
{
    switch(cpu)
    {
        case 0:
            return NUM_GUESTS_CPU0_STATIC;
        case 1:
            return NUM_GUESTS_CPU1_STATIC;
            /*
        case 2:
            return NUM_GUESTS_CPU2_STATIC;
        case 3:
            return NUM_GUESTS_CPU3_STATIC;
            */
    }
    return 255;
}

#endif
