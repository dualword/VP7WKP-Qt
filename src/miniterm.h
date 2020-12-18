/**************************************************************************/
/*                                                                        */
/*   miniterm.h - Mini terminal for the VP7 Way Kool Project.             */
/*                See miniterm.c for details.                             */
/*                                                                        */
/*   Copyright (C) 2001  VP7                                              */
/*                                                                        */
/*   Comments, questions and bugreports should be sent to wkp@vp7.dk      */
/*   You can get the latest version of this program at http://wkp.vp7.dk/ */
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

#define MINITERM_SCROLLBAR_L 0
#define MINITERM_SCROLLBAR_R 1
#define MINITERM_LF "\n"

#define MINITERM_BLACK 0
#define MINITERM_RED 1
#define MINITERM_MAGENTA 2
#define MINITERM_BLUE 3
#define MINITERM_CYAN 4
#define MINITERM_GREEN 5
#define MINITERM_YELLOW 6
#define MINITERM_DARK_RED 7
#define MINITERM_DARK_CYAN 8
#define MINITERM_DARK_BLUE 9
#define MINITERM_DARK_MAGENTA 10
#define MINITERM_DARK_GREEN 11
#define MINITERM_DARK_YELLOW 12
#define MINITERM_BROWN 13
#define MINITERM_GREY 14
#define MINITERM_WHITE 15


GtkWidget *miniterm_create( GtkWidget *, gint, gint, gint );
void miniterm_print( gchar * );
void miniterm_printcol(gchar *, guint8 fg, guint8 bg);
void miniterm_linefeed( void );
void miniterm_testcolors( gchar * );
