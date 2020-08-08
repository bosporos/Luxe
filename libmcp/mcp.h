//
// file mcp.h
// author Maximilien M. Cura
//

#ifndef __LX_LIBMCP
#define __LX_LIBMCP

#include <stdint.h>
#include <stddef.h>

#define LX_MCP_CS0 BCM2835_SPI_CS0
#define LX_MCP_CS1 BCM2835_SPI_CS1

/* clock cap at 2.7V is 1.35 MHz, RPi runs 3.3V, so we'll say 1MHz to be safe
 */
#define LX_MCP_CLOCK_DEFAULT 1000000L

#define LX_SPI_DEV0 0
#define LX_SPI_DEV1 1

#define LX_MCP3004 0
#define LX_MCP3008 1

#define LX_MCP_CMD8_START 0x80
#define LX_MCP_CMD8_SGL 0x40

typedef struct
{
    uint8_t mcp_vers : 1;
    uint8_t spi_which : 1;
    uint8_t pad : 6;

    uint8_t spi_cs;
    uint32_t spi_clock;
} lx_mcp_t;

int lx_mcp_open (lx_mcp_t * mcp,
                 uint8_t vers,
                 uint8_t which_spi,
                 uint8_t cs,
                 uint32_t clock);
int lx_mcp_close (lx_mcp_t * mcp);
int lx_mcp_read_single (lx_mcp_t * mcp,
                        uint8_t channel);

#endif /* !@__LX_LIBMCP */
