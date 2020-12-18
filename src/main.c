/**************************************************************************/
/*                                                                        */
/*   main.c - Main module to the VP7 Way Kool Project                     */
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
#include "counter.h"
#include "source.h"
#include "gfx.h"
#include "ledbox.h"
#include "shield.h"
#include "mca.h"
#include "miniterm.h"
#include "myrandom.h"

gint timestep = 100;  /* in milliseconds */
static GTimer *wkp_timer;

typedef struct _progress_data {
	int timer;
} progress_data;

/* Afslutter programmet civiliseret hvis der kommet et exit signal */
gint close_application( GtkWidget *widget, GdkEvent *event, gpointer data )
{
	gtk_main_quit();
	g_print("\n\n           ***  Shutdown!  ***\n\n");
	return(FALSE);
}

/* Her er hele maskineriet: Denne funktion kaldes med 100 msek mellemrum */
gint progress_timeout( progress_data (*prg_data))
{
	int i;
	guint32 new_counts=0;
	henfald_struct henfald;
	gdouble wkp_time_passed;


	wkp_time_passed = g_timer_elapsed( wkp_timer, 
					   NULL);
	/* debugging */
	/* g_print("Time: %f \n",wkp_time_passed); */

	g_timer_reset(wkp_timer);
	henfald = dodecay((double) wkp_time_passed, src);
	attenuate(&henfald);
	for (i=0; i<3; i++)
		new_counts += henfald.alpha[i].N+henfald.beta[i].N+henfald.gamma[i].N;
	/*printf("New Counts: %d\n",new_counts);*/
	if (new_counts < 0)
		new_counts = 0;
	i = counter_update (new_counts);
/* ledbox_setleds(myrandom(256));*/
	return (TRUE);
}


int main( int argc, char *argv[] )
{
 
  GtkWidget *window, *mainbox, *box, *cshbox;
  progress_data *prg_data;

  /* This has to be called first! */
  gtk_init( &argc, &argv );

  prg_data = g_malloc( sizeof(progress_data));
  initsource();

  /* create the main window, and attach delete_event signal to terminating
     the application */
  window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
  gtk_window_set_policy((GtkWindow *)window,0,0,1); /* NO RESIZE!!!! */
  gtk_window_set_title (GTK_WINDOW (window),
			"The VP7 Way Kool Project version 0.5");
  gtk_signal_connect( GTK_OBJECT (window), "delete_event",
		      GTK_SIGNAL_FUNC (close_application), NULL );
  gtk_container_set_border_width( GTK_CONTAINER (window), 10 );
  gtk_widget_show( window );  /* call it before anything else */

  /* box with counter and shield control */
  cshbox = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start(GTK_BOX(cshbox),init_slide(window),FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(cshbox),
		     counter_create(window,10,TRUE,TRUE,FALSE)
		     ,FALSE,FALSE,0);
/*  gtk_box_pack_end(GTK_BOX(cshbox),init_shields(window),FALSE,TRUE,0);*/
  gtk_widget_show(cshbox);

  /* lowest box with control, picture and counter */
  box = gtk_hbox_new( FALSE, 5 );
/*  gtk_box_pack_start( GTK_BOX(box), ledbox_create(window), FALSE, FALSE, 0 );*/
  gtk_box_pack_start( GTK_BOX(box), miniterm_create(window,MINITERM_SCROLLBAR_R,300,100),
		      TRUE, FALSE, 0 );
  /* print header to miniterm */
  miniterm_print("***************************************\n");
  miniterm_print("         WELCOME TO WKP 0.5\n");
  miniterm_print("***************************************\n");
/*  gtk_box_pack_start(GTK_BOX(box), mca_create(window), TRUE, FALSE, 0);*/
  gtk_box_pack_start( GTK_BOX(box), init_shields(window), FALSE, FALSE, 0 );
  gtk_box_pack_start( GTK_BOX(box), cshbox, TRUE, FALSE, 0 );
  gtk_widget_show( box );

  /* main box with sources, exp. setup, and lower box */
  mainbox = gtk_vbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(mainbox), create_skab( window ), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(mainbox), create_gfx ( window ), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(mainbox), box, FALSE, FALSE, 0);
  gtk_widget_show(mainbox);
  gtk_container_add(GTK_CONTAINER(window), mainbox);

  /* invoking a REAL timer for even MORE realism ... */
  wkp_timer = g_timer_new ();
  g_timer_start(wkp_timer);

  miniterm_print("Loading progresstimer...");

  /* set up timer callback which drives the program */

  prg_data->timer = gtk_timeout_add(timestep, (gpointer) progress_timeout,NULL);
  miniterm_print("Running!\n");
  miniterm_print("Init random seed: ");
  initmyrandom();
  miniterm_printcol("****\n****  Radiation safety warning: Be carefull!\n****\n",
		  MINITERM_RED,MINITERM_WHITE);
  miniterm_print("Everything is set up and ready to use.\nGood luck!\n");

  /* the last function to call */
  gtk_main (); 

  return 0;
}
