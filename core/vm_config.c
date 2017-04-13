#include <vm_config.h>
#include <config.h>
#include <size.h>

struct vm_config vm_conf[] = {
    { 2, SZ_256M, CFG_MEMMAP_GUEST0_ATAGS_OFFSET },
//    { 1, SZ_256M, CFG_MEMMAP_GUEST0_ATAGS_OFFSET + SZ_256M }
};
