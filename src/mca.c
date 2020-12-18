/**************************************************************************/
/*                                                                        */
/*   mca.c - Multi Channel Analyzer module to the VP7 Way Kool Project    */
/*   Copyright (C) 1999, 2000  VP7                                        */
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
#include "mca.h"

GtkWidget *mca_create(GtkWidget *window)
{
  GtkWidget *mcavpixmap;
  GdkPixmap *mcapixmap;
  GdkBitmap *mcamask;
  GtkWidget *frame;
  GtkStyle *style;

  style = gtk_widget_get_style(window);

  mcapixmap = gdk_pixmap_create_from_xpm(window->window,&mcamask,
					 &style->bg[GTK_STATE_NORMAL],
					 "./pixmaps/mca.xpm");
  mcavpixmap = gtk_pixmap_new(mcapixmap,mcamask);
  gtk_widget_show(mcavpixmap);

  /* make frame around all the buttons */
  frame = gtk_frame_new(NULL);
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);

  gtk_container_add( GTK_CONTAINER (frame), mcavpixmap);

  return frame;
}
