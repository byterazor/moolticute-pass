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
* @file ls.c
* @brief source file for the ls command
* @author Dominik Meyer <dmeyer@federationhq.de>
* @copyright 2018 by Dominik Meyer
*/
#include "../libmoolticute-c/src/moolticute.h"
#include "../yaap/src/argparse.h"

/**
* @brief lists all available services and login names of the mooltipass device
*
* @param argc - number of arguments
* @param argv - array of arguments
*/
int ls(int argc, char **argv)
{
  struct arg_parse_ctx *argparse_ctx = argparse_init();
  int ret=0;
  struct mooltipass_service *service = NULL;
  struct mooltipass_credential *credential = NULL;
  struct moolticute_ctx *ctx;
  char service_name[250];

  // command line parameter for service name
  struct arg_str str1 = {
    {ARG_STR,0,0},
    's',
    "service",
    service_name,
    250,
    "service name to list logins of"
  };
  argparse_add_string(argparse_ctx, &str1);

  ret=argparse_parse(argparse_ctx, argc, argv);

  if (ret < 0 )
  {
    return -1;
  }

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

  ret=moolticute_start_memory_management(ctx,1,1);
  if (ret < 0)
  {
    if (ret == M_ERROR_TIMEOUT)
    {
      fprintf(stderr,"ERROR: Timeout waiting for data. You have to acknowledge credential management on your device\n");
      return -1;
    }

    if (ret == M_ERROR_NO_CARD)
    {
      fprintf(stderr,"ERROR: No cryptocard in device. Please insert your card\n");
      return -1;
    }

    if (ret == M_ERROR_NO_MOOLTIPASS_DEVICE)
    {
      fprintf(stderr,"ERROR: No mooltipass device is connected to moolticute daemon. Please connect your device\n");
      return -1;
    }

    if (ret == M_ERROR_DEVICE_LOCKED)
    {
      fprintf(stderr,"ERROR: Mooltipass device is locked. Please unlock it.\n");
      return -1;
    }

    if (ret == M_ERROR_APPROVAL_REQUIRED)
    {
      fprintf(stderr,"ERROR: Please approve action on mooltipass device.\n");
      return -1;
    }

    return -1;
  }


  if (ctx->info.memory->pFirstService == NULL)
  {
    printf("No services configured yet\n");
    return 0;
  }

  printf("|--Services\n");
  service=ctx->info.memory->pFirstService;
  while(service != NULL)
  {
    // print service if this service or all are requested
    if ((str1.base.set == 1 && strcmp(str1.value, service->name) == 0) || str1.base.set == 0)
    {
      printf("|  |--%s\n", service->name);
      credential=service->pFirstCredential;
      while(credential!= NULL)
      {
        printf("|  |   |--%.20s\n", credential->login);
        printf("|  |   |  |--%s\n", credential->description);
        credential=credential->pNext;
      }
    }
    service=service->pNext;
  }

  moolticute_stop_memory_management(ctx, 1);

  return 0;
}
