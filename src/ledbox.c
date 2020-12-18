/**************************************************************************/
/*                                                                        */
/*   ledbox.c - Ledbox Module to the VP7 Way Kool Project                 */
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


#define MAXLED 8

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>


GtkWidget *ledpixmaps[MAXLED];   /* drawable pixmaps to display */
GtkWidget *sellawid;
GdkPixmap *redoff,*redon,*greenoff,*greenon,*sella;
GdkBitmap *rofmask,*ronmask,*gofmask,*gonmask,*sellamask;


void ledbox_setleds(int n)
{
  int d;

  if ((d=n&1)==1)
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[0], redon, rofmask);
  else
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[0], redoff, ronmask);
  if ((d=n&2)==2)
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[1], redon, rofmask);
  else
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[1], redoff, ronmask);
  if ((d=n&4)==4)
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[2], redon, rofmask);
  else
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[2], redoff, ronmask);
  if ((d=n&8)==8)
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[3], redon, rofmask);
  else
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[3], redoff, ronmask);
  if ((d=n&16)==16)
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[4], redon, rofmask);
  else
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[4], redoff, ronmask);
  if ((d=n&32)==32)
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[5], redon, rofmask);
  else
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[5], redoff, ronmask);
  if ((d=n&64)==64)
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[6], redon, rofmask);
  else
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[6], redoff, ronmask);
  if ((d=n&128)==128)
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[7], redon, rofmask);
  else
    gtk_pixmap_set( (GtkPixmap *)ledpixmaps[7], redoff, ronmask);
}


GtkWidget *ledbox_create(GtkWidget *window)
{
  gint i;
  GtkWidget *vbox,*skab;
  GtkStyle *style;

  printf("Ledbox online!\n");
  style = gtk_widget_get_style( window );
  /* create box */
  skab = gtk_hbox_new(FALSE,10);

  /* load pixmaps from files */
  redoff = gdk_pixmap_create_from_xpm(window->window,&rofmask,
				      &style->bg[GTK_STATE_NORMAL],
				      "./pixmaps/redoff.xpm");
  redon = gdk_pixmap_create_from_xpm(window->window,&ronmask,
				      &style->bg[GTK_STATE_NORMAL],
				      "./pixmaps/redon.xpm");

  for (i=0; i<=MAXLED-1; i++) {
    ledpixmaps[i] = gtk_pixmap_new(redoff,rofmask);
    gtk_widget_show(ledpixmaps[i]);
    gtk_box_pack_start( GTK_BOX (skab), ledpixmaps[i], TRUE, FALSE, 0);
  }   

  /* Load Sellafield camera image */
  sella = gdk_pixmap_create_from_xpm( window->window, &sellamask,     
				      &style->bg[GTK_STATE_NORMAL],
				      "./pixmaps/sellafield.xpm" );         
  sellawid = gtk_pixmap_new( sella, sellamask );
  gtk_widget_show( sellawid );

  gtk_widget_show(skab);

  vbox = gtk_vbox_new(FALSE,20);
  gtk_box_pack_start( GTK_BOX (vbox), sellawid, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX (vbox), skab, FALSE, FALSE, 0);
  gtk_widget_show(vbox);

  ledbox_setleds(0);

  return vbox;
}
