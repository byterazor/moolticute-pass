#ifndef __COMMANDS_H__
#define __COMMANDS_H__
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
* @file commands.h
* @brief header file for moolticute-pass commands
* @author Dominik Meyer <dmeyer@federationhq.de>
* @copyright 2018 by Dominik Meyer
*/

int get(int argc, char **argv);                 ///< get the password from the mooltipass device
int ls(int argc, char **argv);                 ///< list services/logins on mooltipass device

#endif
