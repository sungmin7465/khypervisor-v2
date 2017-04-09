#include <vm_map.h>
#include <size.h>
#include "devicetree.h"

struct memdesc_t *vm_mmap[CONFIG_NR_VMS];

uint32_t vm_device_all[MAX_IRQS];

uint32_t vm0_device[] = { 0 };
uint32_t vm1_device[] = { 0 };
uint32_t vm2_device[] = { 0 };
uint32_t vm3_device[] = { 0 };

uint32_t *vm_dev[CONFIG_NR_VMS];

void setup_vm_mmap(void)
{
    vm_mmap[0] = vm_device_md;
    vm_mmap[1] = vm_device_md;

    int i;
    for (i = 32; i < MAX_IRQS; i++) {
        vm_device_all[i - 32] = i;
    }

    vm_dev[0] = vm_device_all;
    vm_dev[1] = vm_device_all;
}
