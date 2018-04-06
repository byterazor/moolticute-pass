#ifndef __X11_CLIPBOARD_H__
#define __X11_CLIPBOARD_H__
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
* @file x11_clipboard.h
* @brief header file for X11 specific structures and functions
* @author Dominik Meyer <dmeyer@federationhq.de>
* @copyright 2018 by Dominik Meyer
*/
#include <X11/Xlib.h>


#define X11_DEFAULT_TIMEOUT 10      ///< default timeout for copying anything to the clipboard

/**
* @brief structure representing the X11 context
*/
struct x11_ctx {
  Display *dpy;     ///< X11 Display
  Window owner;     ///< created window
  Window root;      ///< root window
  int screen;       ///< the screen
  Atom sel;         ///< a selection
  Atom utf8;        ///< utf atom
  XEvent ev;        ///< an event
  XSelectionRequestEvent *sev;  ///< selection request event
  int timeout;      ///< timeout after which to clear clipboard
};



struct x11_ctx *x11_init();                                ///< initialize the x11 context
int x11_put_clipboard(struct x11_ctx *ctx, char *text);    ///< put text into the clipboard

#endif
