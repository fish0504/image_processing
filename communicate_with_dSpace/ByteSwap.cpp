#include <stdio.h>
#include <stdint.h>

inline uint16_t swap16(uint16_t value)
{
    uint16_t ret;
    ret  = value << 8;
    ret |= value >> 8;
    return ret;
}

inline uint32_t swap32(uint32_t value)
{
    uint32_t ret;
    ret  = value              << 24;
    ret |= (value&0x0000FF00) <<  8;
    ret |= (value&0x00FF0000) >>  8;
    ret |= value              >> 24;
    return ret;
}

inline float swap32_f(float value)
{
    float ret;
    ret  = value              << 24;
    ret |= (value&0x0000FF00) <<  8;
    ret |= (value&0x00FF0000) >>  8;
    ret |= value              >> 24;
    return ret;
}
int main(int argc, char* argv[])
{
    uint16_t val16;
    uint32_t val32;
    if (argc != 2) {
        return -1;
    }
    if (sscanf(argv[1], "%x", &val32) == -1) {
        return -1;
    }
    val16 = (uint16_t)(val32>>16);
    printf("0x%04x : 0x%04x\n", val16, swap16(val16));
    printf("0x%08x : 0x%08x\n", val32, swap32(val32));
    return 0;
}