/**************************************************************************/
/*                                                                        */
/*   gfx.c - Graphic module to the VP7 Way Kool Project                   */
/*   Copyright (C) 1999, 2000  VP7                                        */
/*                                                                        */
/*   Comments, questions and bugreports should be adressed to wkp@vp7.dk  */
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
#include <stdlib.h>
#include <stdio.h>
/*#include "cbox.h"*/
#include "miniterm.h"
#include "gfx.h"

static GtkWidget *gfx_wid[5];
static GdkPixmap *holder_pix[2], *detektor_pix, *att_pix[4];
static GdkBitmap *mask;

/*gfloat dist = 1.0; distance between source and detector in mm */

void set_paper_att(int on)
{
  if (on)
    gtk_pixmap_set(( GtkPixmap *)gfx_wid[1], att_pix[1],mask);
  else
    gtk_pixmap_set(( GtkPixmap *)gfx_wid[1], att_pix[0],mask);
}

void set_al_att(int on)
{
  if (on == TRUE)
    gtk_pixmap_set(( GtkPixmap *)gfx_wid[2], att_pix[2],mask);
  else
    gtk_pixmap_set(( GtkPixmap *)gfx_wid[2], att_pix[0],mask);
}

void set_pb_att(int on)
{
  if (on == TRUE)
    gtk_pixmap_set(( GtkPixmap *)gfx_wid[3], att_pix[3],mask);
  else
    gtk_pixmap_set(( GtkPixmap *)gfx_wid[3], att_pix[0],mask);
}

void set_source(int knold)
{
  if (knold)
    gtk_pixmap_set(( GtkPixmap *)gfx_wid[0], holder_pix[1],mask);
  else
    gtk_pixmap_set(( GtkPixmap *)gfx_wid[0], holder_pix[0],mask);
}


GtkWidget *create_gfx (GtkWidget *window)
{
  int i;
  GtkWidget
    *box1, *pixwid,
    *frame;
  GtkStyle *style;
  
  style = gtk_widget_get_style( window );
  miniterm_print("Loading KoolLab pixmaps...");
  holder_pix[0] = gdk_pixmap_create_from_xpm(window->window, &mask,     
					     &style->bg[GTK_STATE_NORMAL],
					     "./pixmaps/holder_tom.xpm" );
  holder_pix[1] = gdk_pixmap_create_from_xpm(window->window, &mask,
					     &style->bg[GTK_STATE_NORMAL],
					     "./pixmaps/holder_med_kilde.xpm");
  att_pix[0] = gdk_pixmap_create_from_xpm( window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   "./pixmaps/att_dummy.xpm" );  
  att_pix[1] = gdk_pixmap_create_from_xpm( window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   "./pixmaps/att_papir.xpm" );  
  att_pix[2] = gdk_pixmap_create_from_xpm( window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   "./pixmaps/att_al.xpm" );  
  att_pix[3] = gdk_pixmap_create_from_xpm( window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   "./pixmaps/att_pb.xpm" );
  detektor_pix = gdk_pixmap_create_from_xpm( window->window, &mask,
					     &style->bg[GTK_STATE_NORMAL],
					     "./pixmaps/detektor_vp7.xpm" ); 
  miniterm_print("completed.\n");
  
  miniterm_print("Make corresponding pixmapswidgets... ");
  /* GTK Criticals kommer herfra !!!!! */
  gfx_wid[0] = gtk_pixmap_new( holder_pix[0], mask );
  gtk_widget_show( gfx_wid[0] );
  gfx_wid[4] = gtk_pixmap_new( detektor_pix, mask );              
  gtk_widget_show( gfx_wid[4] );
  for (i=1; i < 4; i++) {
    gfx_wid[i] = gtk_pixmap_new( att_pix[0], mask ); /* init with dummy att. */
    gtk_widget_show( gfx_wid[i] ); 
  }
  /* HERTIL!!!! */
  miniterm_print("completed.\n");
  miniterm_print("Setup VP7 KoolLab..."); 
  box1 = gtk_hbox_new(FALSE, 0);
  pixwid = gtk_event_box_new();
  gtk_container_add( GTK_CONTAINER(pixwid), gfx_wid[0] );
  gtk_box_pack_start(GTK_BOX(box1), pixwid, FALSE, FALSE, 0);
  gtk_widget_show( pixwid );
  for (i = 1; i < 4; i++) {
    pixwid =  gtk_event_box_new();
    gtk_container_add( GTK_CONTAINER(pixwid), gfx_wid[i] );
    gtk_box_pack_start(GTK_BOX(box1), pixwid, FALSE, FALSE, 0);
    gtk_widget_show( pixwid );
    }
  pixwid = gtk_event_box_new();
  gtk_container_add( GTK_CONTAINER(pixwid), gfx_wid[4] );
  gtk_box_pack_start(GTK_BOX(box1), pixwid, FALSE, FALSE, 0);
  gtk_widget_show( pixwid );
  gtk_widget_show(box1);  
  frame = gtk_frame_new(NULL);      /* add a frame */
  gtk_frame_set_label( GTK_FRAME(frame), " VP7 KoolLab(tm) " );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);
  gtk_container_add (GTK_CONTAINER (frame), box1);
  miniterm_print("completed.\n");
  
  return(frame);
}

