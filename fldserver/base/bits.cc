//
// Created by hussa on 5/20/2021.
//

#include "bits.h"

namespace base::bits
{
int
Log2Floor(uint32_t n)
{
    return 31 - CountLeadingZeroBits(n);
}

int
Log2Ceiling(uint32_t n)
{
    // When n == 0, we want the function to return -1.
    // When n == 0, (n - 1) will underflow to 0xFFFFFFFF, which is
    // why the statement below starts with (n ? 32 : -1).
    return (n ? 32 : -1) - CountLeadingZeroBits(n - 1);
}
#if IS_WIN
uint32_t
CountLeadingZeroBits32(uint32_t x)
{
    return CountLeadingZeroBits(x);
}

uint64_t
CountLeadingZeroBits64(uint64_t x)
{
    return CountLeadingZeroBits(x);
}
#endif

size_t
CountLeadingZeroBitsSizeT(size_t x)
{
    return CountLeadingZeroBits(x);
}

size_t
CountTrailingZeroBitsSizeT(size_t x)
{
    return CountTrailingZeroBits(x);
}

}  // namespace base::bits