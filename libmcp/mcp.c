#include <bcm2835.h>
#include "mcp.h"
#include <errno.h>

int lx_mcp_open (lx_mcp_t * mcp,
                 uint8_t vers,
                 uint8_t which_spi,
                 uint8_t cs,
                 uint32_t clock)
{
    if ((cs != LX_MCP_CS0 && cs != LX_MCP_CS1) || (vers > 1))
        return EINVAL;

    if (!bcm2835_spi_begin ()) {
        return EACCES;
    }
    mcp->spi_cs = cs;
    bcm2835_spi_chipSelect (mcp->spi_cs);
    /* active low */
    bcm2835_spi_setChipSelectPolarity (mcp->spi_cs, 0);
    mcp->spi_clock = clock;
    bcm2835_spi_set_speed_hz (mcp->spi_clock);
    bcm2835_spi_setDataMode (0);

    mcp->mcp_vers = vers;

    return 0;
}

int lx_mcp_close (lx_mcp_t * mcp)
{
    if (mcp == NULL)
        return EINVAL;
    bcm2835_spi_end (mcp->spi_which);
    return 0;
}

uint16_t lx_mcp_read_single (lx_mcp_t * mcp, uint8_t channel)
{
    if (channel & 0xf8)
        return ECHRNG;
    uint8_t command[3] = { LX_MCP_CMD8_START | LX_MCP_CMD8_SGL, 0, 0 };
    command[0] |= (channel << 3);
    uint8_t response[3] = { 0, 0, 0 };

    bcm2835_spi_transfernb (mcp->spi_which, &command, &response, 3);
    uint16_t read = ((response[0] & 0x01) << 9)
        | ((response[1] & 0xff) << 1)
        | ((response[2] & 0x80) >> 7);

    return read;
}
