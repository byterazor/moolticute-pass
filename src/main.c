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
* @file main.c
* @brief Main C Source file for the moolticute-pass application
* @author Dominik Meyer <dmeyer@federationhq.de>
* @copyright 2018 by Dominik Meyer
*/
#include <stdlib.h>
#include <stdio.h>
#include "commands.h"
#include "../yaap/src/argparse.h"

/**
* @brief main function of the moolticute-pass application
*
* @param argc - number of command line arguments
* @param argv - array of command line arguments
*
* @return standard return value
*/
int main(int argc, char **argv)
{
  // initialize argument parser context
  struct arg_parse_ctx *argparse_ctx = argparse_init();

  // command-line get command
  struct arg_parse_cmd get_cmd = {
    {ARG_CMD,1,0},                  // set it mandatory, all commands should be mandatory
    0,
    "get",
    "get a password from mooltipass device",
    &get
  };
  argparse_add_command(argparse_ctx, &get_cmd);


  // command-line ls command
  struct arg_parse_cmd ls_cmd = {
    {ARG_CMD,1,0},                  // set it mandatory, all commands should be mandatory
    0,
    "ls",
    "list services/logins on mooltipass device",
    &ls
  };
  argparse_add_command(argparse_ctx, &ls_cmd);

  // parse the command line, this function calls the correct function for each command
  int ret=argparse_parse(argparse_ctx, argc, argv);

  // free the argument parser context
  argparse_free(argparse_ctx);

  return ret;
}
