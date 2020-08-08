#include "mcp.h"

#include <stdio.h>

int main ()
{
    lx_mcp_t mcp;
    lx_mcp_open (&mcp, LX_MCP3008, LX_SPI_DEV0, LX_MCP_CS0, LX_MCP_CLOCK_DEFAULT);
    uint16_t adc = lx_mcp_read_single (&mcp, 0);
    printf ("Value: %hu\n", adc);
    return 0;
}
