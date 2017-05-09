#include <stdio.h>
#include <vdev.h>
#include <io.h>
#include <core/scheduler.h>
#include <irq-chip.h>
#include <string.h>
#include <arch/irq.h>
#include <core/vm/vm.h>

#define CCU_BASE    0x01c20000
#define CCU_ADDR(x) (CCU_BASE + x)

uint32_t dummy[2];

void sprintbin9(char *str, uint32_t num)
{
    int i;
    for (i = 0; i < 9; i++) {
        str[8-i] = '0' + ((num >> i) & 0x1);
    }
    str[9] = 0;
}

int32_t vccu_read(void *pdata, uint32_t offset);
int32_t vccu_write(void *pdata, uint32_t offset, uint32_t *addr);
int32_t vccu_create(void **pdata);

struct vdev_module vccu = {
    .name   = "vdev_ccu",
    .base   = CCU_BASE,
    .size   = 4096,
    .read   = vccu_read,
    .write  = vccu_write,
    .create = vccu_create,
};

int32_t vccu_read(void *pdata, uint32_t offset)
{
    if (offset == 0xCC) {
        uint32_t vmid = get_current_vm()->vmid;
        uint32_t value = readl(CCU_ADDR(offset));
        char value_bin[10], dummy_bin[10];

        sprintbin9(value_bin, value);
        sprintbin9(dummy_bin, dummy[vmid]);

        printf("** usb_clk read (by vm%d) : 0b%s (dummy : 0b%s)\n", vmid, value_bin, dummy_bin);
        return dummy[vmid];
    }

    return readl(CCU_ADDR(offset));
}

int32_t vccu_write(void *pdata, uint32_t offset, uint32_t *addr)
{
    uint32_t value = readl(addr);

    if (offset == 0xCC) {
        uint32_t vmid = get_current_vm()->vmid;

        uint32_t prev = readl(CCU_ADDR(0xCC));
        char prev_bin[10], value_bin[10];

        sprintbin9(prev_bin, prev);

        uint32_t mask = 0x21 << (2 - vmid);

        prev &= mask;
        value &= ~mask;
        dummy[vmid] = value;
        value |= prev;

        sprintbin9(value_bin, value);

        printf("** usb_clk write (by vm%d) : 0b%s -> 0b%s\n",vmid, prev_bin, value_bin);

    }

    writel(value, CCU_ADDR(offset));
    return 0;
}

int32_t vccu_create(void **pdata)
{
    return 0;
}

hvmm_status_t vdev_ccu_init()
{
    hvmm_status_t result = HVMM_STATUS_BUSY;
    vdev_register(&vccu);
    printf("vdev registered:'%s'\n", vccu.name);

    return result;
}

vdev_module_init(vdev_ccu_init);
