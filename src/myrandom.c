/**************************************************************************/
/*                                                                        */
/*   myrandom.c  -  Returns a random number between 0 and range           */
/*   Copyright (C) 1999, 2000  VP7                                        */
/*                                                                        */
/*   Comments, questions and bugreports should be sent to alex@vp7.dk     */
/*                                                                        */
/*   This program is free software; you can redistribute it and/or modify */
/*   it under the terms of the GNU General Public License as published by */
/*   the Free Software Foundation; either version 2 of the License, or    */
/*   (at your option) any later version.                                  */
/*                                                                        */
/*   This program is distributed in the hope that it will be useful,      */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/*   GNU General Public License for more details.                         */
/*                                                                        */
/*   You should have received a copy of the GNU General Public License    */
/*   along with this program; if not, write to the                        */
/*       Free Software Foundation, Inc.,                                  */
/*       59 Temple Place, Suite 330,                                      */
/*       Boston, MA  02111-1307                                           */
/*       USA                                                              */
/*                                                                        */
/**************************************************************************/

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "miniterm.h"

/* initialize random seed from system clock. */
void initmyrandom()
{
  time_t tp;
  gchar *text;

  /* get system time */
  tp = time(NULL);

  text = malloc(10);
  g_snprintf(text,10,"%d.",(int)tp);
  miniterm_print(text);
  miniterm_linefeed();
  g_free(text);
  /* use it as seed */
  srand(tp);
}

int myrandom(int range)
{
  double temp;
  
  temp = ((double)range)/((double)RAND_MAX);
  return (int)(random()*temp);
}
