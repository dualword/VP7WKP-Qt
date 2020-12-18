/**************************************************************************/
/*                                                                        */
/*   toolbox.c - Toolbox (source) module to the VP7 Way Kool Project      */
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "toolbox.h"
#include "source.h"

GtkWidget *popwin;      /* main popup window */
GtkWidget *okbutton;    /* ok-button */
GtkWidget *message;     /* text label */
GtkWidget *mybox;       /* vertical container box */

extern gint src;


gint closetoolbox(GtkWidget *widget, gpointer *data)
{
  gtk_grab_remove( popwin );
  gtk_object_destroy( GTK_OBJECT(popwin) );
  return 0;
}

void setupsource(GtkWidget *widget, gpointer *data)
{

  /* main window */
  popwin = gtk_window_new( GTK_WINDOW_TOPLEVEL );
  gtk_window_set_title( GTK_WINDOW (popwin),"VP7 Source ToolBox");
  gtk_widget_set_usize( popwin, 200, 100);
  gtk_signal_connect( GTK_OBJECT (popwin), "delete_event",
		      GTK_SIGNAL_FUNC (closetoolbox), NULL);
  gtk_grab_add( popwin );  /* lock mouse/kbd events */
  gtk_container_border_width (GTK_CONTAINER(popwin),5);
  /* button */
  okbutton = gtk_button_new_with_label(" OK, I'll be back later... ");
  gtk_signal_connect( GTK_OBJECT (okbutton), "clicked",
		      GTK_SIGNAL_FUNC(closetoolbox), NULL);
  gtk_widget_show( okbutton );
  /* label */
  message = gtk_label_new(" This feature has not\n been implemented yet. ");
  gtk_widget_show( message );
  /* vertical box */
  mybox = gtk_vbox_new(FALSE,10);
  gtk_box_pack_start( GTK_BOX (mybox), message, FALSE, FALSE, 10);
  gtk_box_pack_start( GTK_BOX (mybox), okbutton, FALSE, FALSE, 10); 
  gtk_container_add(GTK_CONTAINER(popwin), mybox);

  gtk_widget_show( mybox);
  gtk_widget_show( popwin );
}
