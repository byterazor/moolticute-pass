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
* @file x11_clipboard.c
* @brief source file for all x11 functions
* @author Dominik Meyer <dmeyer@federationhq.de>
* @copyright 2018 by Dominik Meyer
*/
#include "x11_clipboard.h"
#include <X11/Xutil.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/**
* @brief initialize the x11 context
*
* @return an initialized x11_ctx structure
*/
struct x11_ctx *x11_init()
{
  struct x11_ctx *ctx = (struct x11_ctx *) calloc(sizeof(struct x11_ctx), 1);

  // open the X11 display
  ctx->dpy = XOpenDisplay(NULL);
  if (!ctx->dpy)
  {
      fprintf(stderr, "Could not open X display\n");
      return NULL;
  }

  ctx->screen = DefaultScreen(ctx->dpy);
  ctx->root = RootWindow(ctx->dpy, ctx->screen);

  /* We need a window to receive messages from other clients. */
  ctx->owner = XCreateSimpleWindow(ctx->dpy, ctx->root, -10, -10, 1, 1, 0, 0, 0);
  XSelectInput(ctx->dpy, ctx->owner, SelectionClear | SelectionRequest);

  ctx->sel = XInternAtom(ctx->dpy, "CLIPBOARD", False);
  ctx->utf8 = XInternAtom(ctx->dpy, "UTF8_STRING", False);

  XMapWindow(ctx->dpy, ctx->owner);
  XFlush(ctx->dpy);

  ctx->timeout = X11_DEFAULT_TIMEOUT;

  return ctx;
}

/**
* @brief deny a clipboard request
*
* @param ctx - the x11 context
*/
void send_no(struct x11_ctx *ctx)
{
    XSelectionEvent ssev;
    char *an;

    if (ctx == NULL)
    {
      return;
    }


    an = XGetAtomName(ctx->dpy, ctx->sev->target);
    if (an)
        XFree(an);

    /* All of these should match the values of the request. */
    ssev.type = SelectionNotify;
    ssev.requestor = ctx->sev->requestor;
    ssev.selection = ctx->sev->selection;
    ssev.target = ctx->sev->target;
    ssev.property = None;  /* signifies "nope" */
    ssev.time = ctx->sev->time;

    XSendEvent(ctx->dpy, ctx->sev->requestor, True, NoEventMask, (XEvent *)&ssev);
}

/**
* @brief send an utf8 string on a clipboard request
*
* @param ctx - the x11 context
* @param text - text to send on clipboard request
*/
void send_utf8(struct x11_ctx *ctx, char *text)
{
    XSelectionEvent ssev;
    char *an;

    an = XGetAtomName(ctx->dpy, ctx->sev->property);
    printf("Sending data to window 0x%lx, property '%s'\n", ctx->sev->requestor, an);
    if (an)
        XFree(an);

    XChangeProperty(ctx->dpy, ctx->sev->requestor, ctx->sev->property, ctx->utf8, 8, PropModeReplace,
                    (const unsigned char*) text, strlen(text));

    ssev.type = SelectionNotify;
    ssev.requestor = ctx->sev->requestor;
    ssev.selection = ctx->sev->selection;
    ssev.target = ctx->sev->target;
    ssev.property = ctx->sev->property;
    ssev.time = ctx->sev->time;

    XSendEvent(ctx->dpy, ctx->sev->requestor, True, NoEventMask, (XEvent *)&ssev);
}


/**
* @brief request the clipboard and wait for requests
*
* @param ctx - x11 context
* @param text - the text to copy to clipboard
*
* @return 0  - everything is fine
* @return <0 - error occured
*/
int x11_put_clipboard(struct x11_ctx *ctx, char *text)
{
  int x11_fd;
  fd_set in_fds;
  struct timeval tv;

  if (ctx == NULL || text == NULL )
  {
    return -1;
  }

  XSetSelectionOwner(ctx->dpy, ctx->sel, ctx->owner, CurrentTime);
  x11_fd = ConnectionNumber(ctx->dpy);

  while(1)
  {
    FD_ZERO(&in_fds);
    FD_SET(x11_fd, &in_fds);

    // Set our timer.  One second sounds good.
    tv.tv_usec = 0;
    tv.tv_sec = 20;
    // Wait for sX Event or a Timer
    if (select(x11_fd+1, &in_fds, 0, 0, &tv))
    {
      while(XPending(ctx->dpy))
      {
        XNextEvent(ctx->dpy, &ctx->ev);
        switch (ctx->ev.type)
        {
            case SelectionClear:
                return 1;
                break;
            case SelectionRequest:
                ctx->sev = (XSelectionRequestEvent*)&ctx->ev.xselectionrequest;
                /* Property is set to None by "obsolete" clients. */
                if (ctx->sev->target != ctx->utf8 || ctx->sev->property == None)
                    send_no(ctx);
                else
                    send_utf8(ctx, text);
                break;
        }
      }
    }
    else
    {
      break;
    }

  }
  return 0;
}
