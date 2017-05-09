#include <stdio.h>
#include <vdev.h>
#include <io.h>
#include <core/scheduler.h>
#include <irq-chip.h>
#include <string.h>
#include <arch/irq.h>
#include <core/vm/vm.h>

#define USB_BASE    0x01c1c000
#define USB_ADDR(x) (USB_BASE + x)

int32_t vusb_read(void *pdata, uint32_t offset);
int32_t vusb_write(void *pdata, uint32_t offset, uint32_t *addr);
int32_t vusb_create(void **pdata);

struct vdev_module vusb = {
    .name   = "vdev_usb2",
    .base   = USB_BASE,
    .size   = 4096,
    .read   = vusb_read,
    .write  = vusb_write,
    .create = vusb_create,
};

int32_t vusb_read(void *pdata, uint32_t offset)
{
    uint32_t value = readl(USB_ADDR(offset));

    if (offset >= 0x000) {
        uint32_t vmid = get_current_vm()->vmid;

        printf("^^ usb2 read  (by vm %d) 0x%03x : 0x%08x\n", vmid, offset, value);
    }

    return value;
}

int32_t vusb_write(void *pdata, uint32_t offset, uint32_t *addr)
{
    uint32_t value = readl(addr);

    if (offset >= 0x000) {
        uint32_t vmid = get_current_vm()->vmid;
        printf("^^ usb2 write (by vm %d) 0x%03x : 0x%08x\n", vmid, offset, value);
    }

    writel(value, USB_ADDR(offset));
    return 0;
}

int32_t vusb_create(void **pdata)
{
    return 0;
}

hvmm_status_t vdev_usb_init()
{
    hvmm_status_t result = HVMM_STATUS_BUSY;
    vdev_register(&vusb);
    printf("vdev registered:'%s'\n", vusb.name);

    return result;
}

vdev_module_init(vdev_usb_init);
