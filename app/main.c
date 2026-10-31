#include <stdbool.h>
#include <stdint.h>

/* Build-time sanity checks for the cross-compilation environment */
_Static_assert(sizeof(uint32_t) == 4, "uint32_t must be 4 bytes");
_Static_assert(sizeof(void *) == 4, "pointer must be 4 bytes on Cortex-M4");
_Static_assert(sizeof(float) == 4, "float must be 4 bytes (FPv4-SP)");

int main(void)
{
    for (;;)
        ;
}
