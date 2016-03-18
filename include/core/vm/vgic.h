#ifndef __VGIC_H__
#define __VGIC_H__

#include <stdlib.h>

// FIXME(casionwoo) : This TILinesNumber should be modified like GICv2.TILinesNumber as dynamically
#define TILinesNumber 4
#define NUM_MAX_VIRQS   160

#define NUM_STATUS_WORDS    (NUM_MAX_VIRQS / 32)
#define VGICE_NUM_ISCPENDR (TILinesNumber + 1)
#define VGICE_NUM_ISCENABLER (TILinesNumber + 1)
#define VGICE_NUM_ISCACTIVER (TILinesNumber + 1)
#define VGICE_NUM_IPRIORITYR (8*(TILinesNumber + 1))
#define VGICE_NUM_ITARGETSR (8*(TILinesNumber + 1))
#define VGICE_NUM_ICFGR (2*(TILinesNumber + 1))
#define VGICE_NUM_SPISR (TILinesNumber + 1)
#define VGICE_NUM_CPENDSGIR (TILinesNumber + 1)
#define VGICE_NUM_SPENDSGIR (TILinesNumber + 1)
#define VGICD_NUM_IGROUPR (TILinesNumber + 1)
#define VGICD_NUM_IENABLER (TILinesNumber + 1)

/* Banked Registers Size */

#define VGICD_BANKED_NUM_IPRIORITYR  8
#define VGICD_BANKED_NUM_ITARGETSR  8
#define VGICD_BANKED_NUM_CPENDSGIR  VGICE_NUM_CPENDSGIR
#define VGICD_BANKED_NUM_SPENDSGIR  VGICD_NUM_SPENDSGIR

struct gicd_regs {
    uint32_t CTLR;                              /*0x000 RW*/
    uint32_t TYPER;                             /*      RO*/
    uint32_t IIDR;                              /*      RO*/

    uint32_t IGROUPR[VGICD_NUM_IGROUPR];        /* 0x080 */
    uint32_t ISCENABLER[VGICE_NUM_ISCENABLER];  /* 0x100, ISENABLER/ICENABLER */
    uint32_t ISCPENDR[VGICE_NUM_ISCPENDR];      /* 0x200, ISPENDR/ICPENDR */
    uint32_t ISCACTIVER[VGICE_NUM_ISCACTIVER];  /* 0x300, ISACTIVER/ICACTIVER */
    uint32_t IPRIORITYR[VGICE_NUM_IPRIORITYR];  /* 0x400 */
    uint32_t ITARGETSR[VGICE_NUM_ITARGETSR];    /* 0x800 [0]: RO, Otherwise, RW */
    uint32_t ICFGR[VGICE_NUM_ICFGR];            /* 0xC00 */

    /* Cortex-A15 */
    /* 0xD00 GICD_PPISR RO */
    /* 0xD04 ~ 0xD1C GICD_SPISRn RO */

    uint32_t NSACR[64]; /* 0xE00 */
    uint32_t SGIR; /* 0xF00 WO */
    //uint32_t CPENDSGIR[VGICE_NUM_CPENDSGIR];
    /* 0xF10 CPENDSGIR 0xF20 SPENDGIR */

    /* 0xFD0 ~ 0xFFC RO Cortex-A15 PIDRn, CIDRn */
};

struct gicd_regs_banked {
    uint32_t IGROUPR;                                   //0
    uint32_t ISCENABLER;                                //0
    uint32_t ISCPENDR;                                  //0
    uint32_t ISCACTIVER;                                //0
    uint32_t IPRIORITYR[VGICD_BANKED_NUM_IPRIORITYR];   //0~7
    uint32_t ITARGETSR[VGICD_BANKED_NUM_ITARGETSR];     //0~7
    uint32_t ICFGR;                                     //1
    uint32_t CPENDSGIR[VGICD_BANKED_NUM_CPENDSGIR];     //n
};

struct vgic {
    struct gicd_regs gicd_regs;
    struct gicd_regs_banked gicd_regs_banked;
};

void vgic_init(struct vgic *vgic);

#endif /* __VGIC_H__ */

