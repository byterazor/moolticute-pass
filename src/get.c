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
* @file get.c
* @brief source file for the get command
* @author Dominik Meyer <dmeyer@federationhq.de>
* @copyright 2018 by Dominik Meyer
*/
#include "../libmoolticute-c/src/moolticute.h"
#include "../yaap/src/argparse.h"
#include "x11_clipboard.h"
#include "../config.h"

/**
* @brief get the password of a services and login
*
* @param argc - number of command line arguments
* @param argv - array of command line arguments
*
* @return standard return value
*/
int get(int argc, char **argv)
{
  struct arg_parse_ctx *argparse_ctx = argparse_init();
  struct moolticute_ctx *ctx;
  int ret=0;
  char password[500];
  char service[250];
  char user[250];

  struct arg_str str1 = {
    {0,1,0},
    's',
    "service",
    service,
    250,
    "service name of the requested password"
  };
  argparse_add_string(argparse_ctx, &str1);

  struct arg_str str2 = {
    {0,1,0},
    'u',
    "user",
    user,
    250,
    "user name for the requested password"
  };
  argparse_add_string(argparse_ctx, &str2);


/**
* if X11 clipboard is enabled add argument for selecting it
*/
#ifdef HAVE_X11
  struct arg_flag x11 = {
    {ARG_FLAG,0,0},
    'c',
    "clipboard",
    "copy password to X11 clipboard instead stdout",
    NULL
  };
  argparse_add_flag(argparse_ctx, &x11);
#endif

  ret=argparse_parse(argparse_ctx, argc, argv);

  if (ret < 0 )
  {
    return -1;
  }

/**
* if X11 clipboard is enabled check if is selected and init x11 context
*/
#ifdef HAVE_X11
struct x11_ctx *x11ctx;

if (x11.base.set == 1)
{
    // x11 is selected
    x11ctx = x11_init();
}
#endif

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

  ret=moolticute_request_password(ctx, service, user, password,30);
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

#ifdef HAVE_X11
  if (x11.base.set == 1)
  {
    x11_put_clipboard(x11ctx, password);
    return 0;
  }
#endif

  printf("%s", password);


  return 0;
}
