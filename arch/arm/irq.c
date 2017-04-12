#include <stdio.h>
#include <arch/irq.h>
#include <config.h>
#include <core/vm/vcpu.h>
#include <arch/armv7.h>
#include <irq-chip.h>
#include <types.h>
#include <core/scheduler.h>

#define VIRQ_MIN_VALID_PIRQ     16
#define VIRQ_NUM_MAX_PIRQS      MAX_IRQS

static irq_handler_t irq_handlers[MAX_IRQS];

hvmm_status_t do_irq(struct core_regs *regs)
{
    uint32_t pcpu = smp_processor_id();
    uint32_t irq = irq_hw->ack();
    uint32_t cpuid = (irq >> 10) & 0x7;
    irq &= 0x3FF;

    if (pcpu == 1)
        printf("CPU[%d] IRQ[%d]\n", pcpu, irq);
    if (irq < 16)
        printf("cpuid: %d\n", cpuid);
    irq_hw->eoi(irq);

    if (irq_handlers[irq](irq, regs, 0) != VM_IRQ) {
        irq_hw->dir(irq);
    }

    return HVMM_STATUS_SUCCESS;
}

void irq_init()
{
    uint32_t hcr;

    set_irqchip_type();

    hcr = read_cp32(HCR);
    hcr |= HCR_BIT(IMO) | HCR_BIT(FMO);
    write_cp32(hcr, HCR);

    irq_handler_init(irq_handlers);
}

void register_irq_handler(uint32_t irq, irq_handler_t handler, uint8_t polarity)
{
    if (irq < MAX_IRQS) {
        irq_handlers[irq] = handler;
    }
    irq_hw->set_irq_type(irq, polarity);
    irq_hw->enable(irq);
}

