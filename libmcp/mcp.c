#include <bcm2835.h>
#include "mcp.h"

int lx_mcp_open (lx_mcp_t * mcp,
                 uint8_t vers,
                 uint8_t which_spi,
                 uint8_t cs,
                 uint32_t clock)
{
    if ((cs != LX_MCP_CS0 && cs != LX_MCP_CS1) || (vers | which_spi) > 1)
        return EINVAL;

    mcp->spi_which = which_spi;
    if (!__lx_spictl_begin (mcp->spi_which)) {
        return EACCES;
    }
    mcp->spi_cs = cs;
    __lx_spictl_chipselect (mcp->spi_which, mcp->spi_cs);
    /* active low */
    __lx_spictl_chipselect_polarity (mcp->spi_which, mcp->spi_cs, 0);
    mcp->spi_clock = clock;
    __lx_spictl_setclock (mcp->spi_which, mcp->spi_clock);
    __lx_spictl_datamode (mcp->spi_which, 0);

    mcp->mcp_vers = vers;

    return 0;
}

int lx_mcp_close (lx_mcp_t * mcp)
{
    if (mcp == NULL)
        return EINVAL;
    __lx_spictl_end (mcp->spi_which);
    return 0;
}

uint16_t lx_mcp_read_single (lx_mcp_t * mcp, uint8_t channel)
{
    if (channel & 0xf8)
        return ECHRNG;
    uint8_t command[3] = { LX_MCP_CMD8_START | LX_MCP_CMD8_SGL, 0, 0 };
    command[0] |= (channel << 3);
    uint8_t response[3];

    __lx_spictl_transfernb (mcp->spi_which, &command, &response, 3);
    uint16_t read = ((response[0] & 0x01) << 9)
        | ((response[1] & 0xff) << 1)
        | ((response[2] & 0x80) >> 7);

    return read;
}
