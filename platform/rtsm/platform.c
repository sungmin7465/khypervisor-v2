#include "platform.h"
#include <stdint.h>
#include <drivers/serial_pl01x.h>
#include <drivers/sp804.h>

// TODO(wonseok): moved header files from arch/arm to proper dir.
#include "../../arch/arm/paging.h"

#include <size.h>
#include <arch/gic_regs.h>

#include <arch/armv7.h>

#include <config.h>

#define CFG_HYP_START_ADDRESS      0xF0000000
void platform_init()
{
    uint32_t gic_base = (uint32_t)(get_periphbase() & 0x000000FFFFFFFFFFULL);
    gic_base = CFG_GIC_BASE_PA;

    paging_add_mapping(gic_base + GICD_OFFSET, gic_base + GICD_OFFSET, MT_DEVICE, SZ_4K);
    paging_add_mapping(gic_base + GICC_OFFSET, gic_base + GICC_OFFSET, MT_DEVICE, SZ_8K);
    paging_add_mapping(gic_base + GICH_OFFSET, gic_base + GICH_OFFSET, MT_DEVICE, SZ_4K);
    paging_add_mapping(gic_base + GICV_OFFSET, gic_base + GICV_OFFSET, MT_DEVICE, SZ_8K);

    paging_add_mapping(0x1C050000, 0x1C050000, MT_DEVICE, SZ_4K);

    // add mapping for serial devices
    paging_add_mapping(0x1C090000, 0x1C090000, MT_DEVICE, SZ_4K);
    paging_add_mapping(0x1C0A0000, 0x1C0A0000, MT_DEVICE, SZ_4K);
    paging_add_mapping(0x1C0B0000, 0x1C0B0000, MT_DEVICE, SZ_4K);
    paging_add_mapping(0x1C0C0000, 0x1C0C0000, MT_DEVICE, SZ_4K);

    // for SP804
    paging_add_mapping(0x1C110000, 0x1C110000, MT_DEVICE, SZ_4K);
    paging_add_mapping(0x1C120000, 0x1C120000, MT_DEVICE, SZ_4K);

    paging_add_mapping(0xF0000000, CFG_HYP_START_ADDRESS, MT_WRITEBACK_RW_ALLOC, SZ_256M);
    paging_add_mapping(0x80000000, 0x80000000, MT_WRITEBACK_RW_ALLOC, SZ_256K);
    paging_add_mapping(0x90000000, 0x90000000, MT_WRITEBACK_RW_ALLOC, SZ_256K);
    paging_add_mapping(0xA0000000, 0xA0000000, MT_WRITEBACK_RW_ALLOC, SZ_256K);
}

void console_init()
{
    // TODO(wonseok): add general initialization for console devices.
    serial_init(115200, 24000000);
}

void dev_init()
{
    //sp804_enable();
}
