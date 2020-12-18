/**************************************************************************/
/*                                                                        */
/*   miniterm.c - Mini terminal for the VP7 Way Kool Project.             */
/*                Miniterm is a gtk text widget, where many kind of       */
/*                funny stuff is printed. It's main purpose is            */
/*                entertaining the user, but it is also useful for        */
/*                debugging. Have fun!                                    */
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

#include <gtk/gtk.h>
#include "miniterm.h"

GtkWidget *textbox;
GdkFont *miniterm_font;
GdkColor miniterm_fg, miniterm_bg;

GdkColor colors[16];
GdkColormap *cmap;

GtkWidget *miniterm_create( GtkWidget *window , gint scrollbarright, gint w, gint h )
{
	GtkWidget *vscrollbar, *hbox;
	guint8 i;

	/* initialize some default fonts and colors */
	miniterm_font = gdk_font_load ("-Misc-Fixed-Medium-R-Normal--10-100-75-75-C-60-ISO8859-9");
	miniterm_bg.red = 0xffff;
	miniterm_bg.green = 0xffff;
	miniterm_bg.blue = 0xffff;
	miniterm_fg.red = 0;
	miniterm_fg.green = 0;
	miniterm_fg.blue =0;

	/* VGA 16 palette - or something like that */
	/* This method is just preliminary */
	colors[MINITERM_BLACK].red = 0;
	colors[MINITERM_BLACK].green = 0;
	colors[MINITERM_BLACK].blue = 0;
	colors[MINITERM_RED].red = 0xffff;
	colors[MINITERM_RED].green = 0;
	colors[MINITERM_RED].blue = 0;
	colors[MINITERM_MAGENTA].red = 0xffff;
	colors[MINITERM_MAGENTA].green = 0;
	colors[MINITERM_MAGENTA].blue = 0xffff;
	colors[MINITERM_BLUE].red = 0;
	colors[MINITERM_BLUE].green = 0;
	colors[MINITERM_BLUE].blue = 0xffff;
	colors[MINITERM_CYAN].red = 0;
	colors[MINITERM_CYAN].green = 0xffff;
	colors[MINITERM_CYAN].blue = 0xffff;
	colors[MINITERM_GREEN].red = 0;
	colors[MINITERM_GREEN].green = 0xffff;
	colors[MINITERM_GREEN].blue = 0;
	colors[MINITERM_YELLOW].red = 0xffff;
	colors[MINITERM_YELLOW].green = 0xffff;
	colors[MINITERM_YELLOW].blue = 0;
	colors[MINITERM_DARK_RED].red = 0xa000;
	colors[MINITERM_DARK_RED].green = 0;
	colors[MINITERM_DARK_RED].blue = 0;
	colors[MINITERM_DARK_MAGENTA].red = 0xa000;
	colors[MINITERM_DARK_MAGENTA].green = 0;
	colors[MINITERM_DARK_MAGENTA].blue = 0xa000;
	colors[MINITERM_DARK_BLUE].red = 0;
	colors[MINITERM_DARK_BLUE].green = 0;
	colors[MINITERM_DARK_BLUE].blue = 0xa000;
	colors[MINITERM_DARK_CYAN].red = 0;
	colors[MINITERM_DARK_CYAN].green = 0xa000;
	colors[MINITERM_DARK_CYAN].blue = 0xa000;
	colors[MINITERM_DARK_GREEN].red = 0;
	colors[MINITERM_DARK_GREEN].green = 0xa000;
	colors[MINITERM_DARK_GREEN].blue = 0;
	colors[MINITERM_DARK_YELLOW].red = 0xa000;
	colors[MINITERM_DARK_YELLOW].green = 0xa000;
	colors[MINITERM_DARK_YELLOW].blue = 0;
	colors[MINITERM_BROWN].red = 0xa000;
	colors[MINITERM_BROWN].green = 0x0000;
	colors[MINITERM_BROWN].blue = 0x0000;
	colors[MINITERM_GREY].red = 0xa000;
	colors[MINITERM_GREY].green = 0xa000;
	colors[MINITERM_GREY].blue = 0xa000;
	colors[MINITERM_WHITE].red = 0xffff;
	colors[MINITERM_WHITE].green = 0xffff;
	colors[MINITERM_WHITE].blue = 0xffff;
	/* try to allocate the colors */
	/* doesn't seem to be necessary! */
/*	cmap = gdk_colormap_get_system();
	for ( i=0; i<16; i++ )
		if (!gdk_colormap_alloc_color(cmap, &colors[i],TRUE,TRUE))
		g_print("Failed to allocate color: %d\n",i);*/


	hbox = gtk_hbox_new( FALSE, 0 );

	/* Textbox creates its own adjustments */
	textbox = gtk_text_new( NULL, NULL );
	gtk_widget_set_usize( GTK_WIDGET(textbox), w, h );
 
	if (scrollbarright)
		gtk_box_pack_start( GTK_BOX(hbox), textbox, FALSE, FALSE, 0 );

	/* text widget is not writable by user */
	gtk_text_set_editable( GTK_TEXT(textbox), FALSE);

	/* don't break words */
	gtk_text_set_word_wrap( GTK_TEXT(textbox), FALSE );

	/* add vertical scrollbar */
	vscrollbar = gtk_vscrollbar_new( GTK_TEXT(textbox)->vadj );
	gtk_box_pack_start( GTK_BOX(hbox), vscrollbar, FALSE, FALSE, 0 );

	if (!scrollbarright)
		gtk_box_pack_start( GTK_BOX(hbox), textbox, FALSE, FALSE, 0 );

	gtk_widget_show_all( hbox );

	return hbox;
}

void miniterm_print(gchar *text)
{
	gtk_text_insert( GTK_TEXT(textbox), miniterm_font,
			 &miniterm_fg, &miniterm_bg,
			 text, -1);
}

void miniterm_printcol(gchar *text, guint8 fg, guint8 bg)
{
	gtk_text_insert( GTK_TEXT(textbox) , miniterm_font,
			 &colors[fg], &colors[bg],
			 text, -1 );
}

void miniterm_linefeed()
{
	gtk_text_insert( GTK_TEXT(textbox), NULL, 
			 NULL, NULL, MINITERM_LF, -1 );
}

void miniterm_testcolors( gchar *text )
{
	guint8 i,j;

	for ( i=0; i<16; i++ )
		for ( j=0; j<16; j++ ) {
			miniterm_printcol( " WKP ", j, i );
			if ( !((j+1)%4) )
				miniterm_linefeed();
		}
}
