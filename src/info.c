/*
moolticute-pass

Copyright (C) 2018  Dominik Meyer

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/**
* @file info.c
* @brief source file for the info command
* @author Dominik Meyer <dmeyer@federationhq.de>
* @copyright 2018 by Dominik Meyer
*/
#include "../libmoolticute-c/src/moolticute.h"

/**
* @brief show infos about the mooltipass device
*
* @param argc - number of command line arguments
* @param argv - array of command line arguments
*
* @return standard return value
*/
int info(int argc, char **argv)
{
  struct moolticute_ctx *ctx;
  int ret=0;

  // create moolticute context
  ctx=moolticute_init_ctx();
  if (ctx == NULL)
  {
    fprintf(stderr,"ERROR: can not create library context.\n");
    return -1;
  }

  // connect to moolticute daemon
  ret=moolticute_connect(ctx);
  if (ret < 0)
  {
    fprintf(stderr,"ERROR: can not connect to moolticute daemon. Is daemon running and is environment variable correctly set?\n");
    return -1;
  }

  printf("|--moolticute\n");
  printf("|  |--info\n");
  printf("|  |  |--device\n");
  printf("|  |  |  |--flash_size=%d\n",ctx->info.device.flash_size);
  printf("|  |  |  |--hw_serial=%.8X\n",ctx->info.device.hw_serial);
  printf("|  |  |  |--hw_version=%s\n",ctx->info.device.hw_version);
  printf("|  |  |--status\n");
  printf("|  |  |  |--connected-to-daemon=%d\n",ctx->info.status.connected);
  printf("|  |  |  |--device-locked=%d\n",ctx->info.status.locked);



  return 0;
}
