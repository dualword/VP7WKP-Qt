/**************************************************************************/
/*                                                                        */
/*   counter.h - General purpose counter.                                 */
/*   Copyright (C) 1999, 2000, 2001  VP7                                  */
/*                                                                        */
/*   Comments, questions and bugreports should be adressed to             */
/*   <bassler@vp7.dk>                                                     */  
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
/* counter.c v.0.4.2 */
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "counter.h"

#define MAX_DIGITS 13 // No more than 13 digits are supported
#define MAX_SMOOTHING_ITERATIONS 200

static GtkWidget *digitwidget[MAX_DIGITS],
  *smoothcombo;
static GdkPixmap *digitpixmap[12];
static GdkBitmap *mask;
static GTimer *mytimer;

typedef struct _MyData{
  int digit[MAX_DIGITS];
  guint8 counter_number_of_digits;
  gboolean hold;
  gboolean reset;
  gboolean mode;
  gboolean power;
  guint8 smoothing;
} MyData;

static MyData *pdata;

static gboolean integer_to_string (guint64 a, int str[], int i);

static double calc_activity(guint64 new_counts);

static void initcounter(MyData *pdata,
			const guint8 counter_number_of_digits,
			const gboolean counter_default_state_power,
			const gboolean counter_default_state_mode,
			const gboolean counter_default_state_hold
			);

static char *filename( int n );

static void power_callback( GtkWidget *widget, MyData *data );
static void reset_callback( GtkWidget *widget, MyData *data );
static void hold_callback( GtkWidget *widget, MyData *data );
static void toggle_callback( GtkWidget *widget, MyData *data );
static void smooth_callback( GtkWidget *widget, MyData *data );

static gboolean integer_to_string (guint64 a, int str[], int i) 
{
  if ( a < ( (guint64) pow(10,i))) {
    while ( i > 0 )
      str[i] = (a % ((guint64) pow(10,i)) ) / ( ((guint64) pow(10,--i)) );
    return (TRUE);
  }
  else {
    g_error("Integer too large [integer_to_string]\n");
    return (FALSE);
  }
}

static gdouble calc_activity(guint64 new_counts)
{
  gdouble activity = 0;
  gdouble time_passed;
  guint8 i;
  /* static variables are guaranteed to be initialized to 0 */
  static gdouble temp[MAX_SMOOTHING_ITERATIONS];
  static guint8 index;
  
  time_passed = g_timer_elapsed( mytimer, 
				 NULL);
  /* g_print("Time: %f ---",time_passed); */
  g_timer_reset(mytimer);
  if (!(time_passed == 0)) {
    temp[index++] = new_counts / time_passed;
    if (index >= pdata->smoothing)
      index = 0;
    /* temp is a float. Assuming that new_counts will not
       change significantly from each call */
    for (i = 0; i < pdata->smoothing; i++)
      activity += temp[i];
    activity = activity / pdata->smoothing;
    return activity;
  }
  else { 
    g_warning("Division by zero. [calc_activity]\n");
    return 0;
  }
}

gboolean counter_update(guint32 new_counts)
{
  static guint64 total_counts;
  gboolean p = TRUE; 
  guint i;
  guint count_str[pdata->counter_number_of_digits];
  guint activity_str[pdata->counter_number_of_digits];
  gdouble activity;

  activity = calc_activity(new_counts);
  activity += 0.5;   // for rounding purposes
  
  /* if counter is off: */
  if (!pdata->power)
    for(i=0; i<pdata->counter_number_of_digits; i++)
      gtk_pixmap_set( (GtkPixmap *)digitwidget[i], 
		      digitpixmap[11],mask);

  else {
    /* count when NOT hold and reset: */  
    if (!(pdata->reset && pdata->hold))
      total_counts = total_counts + new_counts;

    /* if reset and counter overflow: */
    if (total_counts >= (guint64) pow(10,pdata->counter_number_of_digits) || (pdata->reset)) 
      total_counts=0;
    
    if (pdata->mode)
      p = integer_to_string((guint64) total_counts,count_str, 
			    pdata->counter_number_of_digits);
    else
      p = integer_to_string((guint64) activity,activity_str, 
			    pdata->counter_number_of_digits);
        
    /* update widgets */
    if (((p && !pdata->hold) || pdata->reset) && pdata->mode)
      for(i=0; i< pdata->counter_number_of_digits; i++)
	gtk_pixmap_set( (GtkPixmap *)digitwidget[i], 
			digitpixmap[(count_str[i])],mask);

    /* when counter is in activity mode: */   
    if ((p || pdata->reset) && !pdata->mode) 
      {
	gtk_pixmap_set( (GtkPixmap *)digitwidget[0], 
			digitpixmap[10],
			mask);
	for(i=1; i< pdata->counter_number_of_digits; i++)
	  gtk_pixmap_set( (GtkPixmap *)digitwidget[i], 
			  digitpixmap[(activity_str[i-1])],
			  mask);
      }
  }

  if (!pdata->hold)
    pdata->reset = FALSE;
  return (p);
}

static void initcounter( MyData *pdata,
			 const guint8 counter_number_of_digits,
			 const gboolean counter_default_state_power,
			 const gboolean counter_default_state_mode,
			 const gboolean counter_default_state_hold
			 )
{
  pdata->counter_number_of_digits = counter_number_of_digits;
  pdata->hold = counter_default_state_hold;   // init hold button 
  pdata->mode = counter_default_state_mode;   // init mode button 
  pdata->power = counter_default_state_power; // init power button 
  pdata->reset = TRUE;                // reset counter on initialization
  pdata->smoothing = 1;               // no smoothing

  if (!counter_update(0))
    g_error("Counter not inititalized [initcounter]");
}

static char *filename( int n )
{
  static char *name[] = {
    "./pixmaps/0.xpm","./pixmaps/1.xpm","./pixmaps/2.xpm","./pixmaps/3.xpm",
    "./pixmaps/4.xpm","./pixmaps/5.xpm","./pixmaps/6.xpm","./pixmaps/7.xpm",
    "./pixmaps/8.xpm","./pixmaps/9.xpm","./pixmaps/enhed.xpm", 
    "./pixmaps/dummy_digit.xpm"
  };
  return (n < 1 || n > 11) ? name[0] : name[n];
}

static void power_callback( GtkWidget *widget, MyData *data )
{
  /* reset counter when power is turned on: */
  if (!(data->power))
    data->reset = TRUE;
  data->power = !(data->power);
}

static void reset_callback( GtkWidget *widget, MyData *data )
{
  data->reset = TRUE;
}

static void hold_callback( GtkWidget *widget, MyData *data )
{
  data->hold=!(data->hold);
}

static void toggle_callback( GtkWidget *widget, MyData *data )
{
  data->mode=!(data->mode);
  /* TRUE = counter; FALSE = activity; */
  /* if hold is pressed and counter is toggled from 
     activity mode to counter mode, then update counter */ 
  if ((data->mode) && (data->hold)) {
    data->hold = FALSE;
    counter_update(0);
    data->hold = TRUE;
  }
}

static void smooth_callback( GtkWidget *widget, MyData *data )
{
  unsigned int a;
  char b[22];

  strcpy(b,gtk_entry_get_text (GTK_ENTRY (GTK_COMBO(smoothcombo)->entry)));
  if (!isdigit(b[0]))
    pdata->smoothing = 1;
  else {
    sscanf(b, "%i%*s", &a);
    pdata->smoothing = a * 10;
  }
}

GtkWidget *counter_create (GtkWidget *window,
			   const guint8 counter_number_of_digits,
			   const gboolean counter_default_state_power,
			   const gboolean counter_default_state_mode,
			   const gboolean counter_default_state_hold
			   )
{
  int signed i;
  GtkWidget
    *box1, *box2, *box3, *mainbox,
    *resetbutton, *holdbutton, *togglebutton, *quitbutton,
    *frame, *digit, *label;
  GtkTooltips *tooltips;
  GtkStyle *style;
  GList *items = NULL;

  /* do some basic checking */
  if (!G_HAVE_GINT64)
    g_error("64-bit integers unavailable on this platform!\n");

  if (counter_number_of_digits > MAX_DIGITS)
    g_error("No more than %i digits are supportet. (You can't be serious!?)\n", 
	    MAX_DIGITS);
  pdata = g_malloc( sizeof(MyData));
  g_print(" Number of digits: %d\n", counter_number_of_digits);
  style = gtk_widget_get_style( window );

  /* load pixmaps */
  for (i=0; i < 12; i++)
    digitpixmap[i] = gdk_pixmap_create_from_xpm( window->window, &mask,
						 &style->bg[GTK_STATE_NORMAL],
						 filename(i) );                  
  /* create pixmapwidget to contain pixmaps */
  for (i=0; i < counter_number_of_digits; i++) {
    digitwidget[i] = gtk_pixmap_new( digitpixmap[i], mask );                     
    gtk_widget_show( digitwidget[i] );
  }

  /* box for a label and the digits */
  box1 = gtk_hbox_new(FALSE, 0);
   
  label = gtk_label_new(" UniversalCounterBox \n by VP7.");
  gtk_box_pack_start(GTK_BOX(box1), label, TRUE, TRUE, 0);
  gtk_widget_show(label);

  /* add some digits to contain the pixmap widget */
  for (i = (counter_number_of_digits - 1); i > -1; i--) {
    digit =  gtk_event_box_new();
    gtk_container_add( GTK_CONTAINER(digit), digitwidget[i] );
    gtk_box_pack_start(GTK_BOX(box1), digit, TRUE, TRUE, 0);
    gtk_widget_show( digit );
  }
  gtk_widget_show(box1); /* finished box1 */
  
  /* main box for box1 and box3 */
  box2 = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box2),box1, FALSE, FALSE, 0);
  gtk_widget_show(box1);

  /* button box (box3)*/
  box3 = gtk_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(box2),box3,FALSE, FALSE,0);
  tooltips = gtk_tooltips_new (); 
  resetbutton = gtk_button_new_with_label ("  RESET  ");  
  gtk_signal_connect (GTK_OBJECT (resetbutton), "clicked",
		      GTK_SIGNAL_FUNC (reset_callback), (gpointer) pdata);
  gtk_box_pack_end(GTK_BOX(box3), resetbutton, FALSE, FALSE, 0);
  gtk_widget_show(resetbutton);
  gtk_tooltips_set_tip (tooltips, resetbutton, "Reset the display", NULL);

  holdbutton = gtk_toggle_button_new_with_label("  HOLD  ");
  gtk_signal_connect (GTK_OBJECT (holdbutton), "clicked",
		      GTK_SIGNAL_FUNC (hold_callback), (gpointer) pdata);
  gtk_box_pack_end(GTK_BOX(box3), holdbutton, FALSE, FALSE, 0);
  gtk_widget_show(holdbutton);
  gtk_tooltips_set_tip (tooltips, holdbutton, "Freeze the display", NULL);

  togglebutton = gtk_toggle_button_new_with_label("  MODE  ");
  gtk_signal_connect (GTK_OBJECT (togglebutton), "clicked",
		      GTK_SIGNAL_FUNC (toggle_callback), (gpointer) pdata);

  gtk_box_pack_end(GTK_BOX(box3), togglebutton, FALSE, FALSE, 0);
  gtk_widget_show(togglebutton);
  gtk_tooltips_set_tip (tooltips, togglebutton, "Select Counter/Activity mode", NULL);

  /* Add a combo:
     You may change the first integer in the list item, if you desire.
     The string is beeing interpreted in smooth_callback function. */
  items = g_list_append (items, "No activity smoothing");
  items = g_list_append (items, "5 sec.");
  items = g_list_append (items, "10 sec.");
  items = g_list_append (items, "20 sec.");
  smoothcombo = gtk_combo_new();
  gtk_combo_set_popdown_strings (GTK_COMBO (smoothcombo), items);
  gtk_signal_connect ( GTK_OBJECT (GTK_COMBO (smoothcombo)->entry), "changed",
		       GTK_SIGNAL_FUNC (smooth_callback), (gpointer) pdata);
  // next line looks criminal, but it works fine?
  gtk_editable_set_editable (GTK_EDITABLE (GTK_COMBO (smoothcombo)->entry), FALSE); 
  gtk_box_pack_end(GTK_BOX(box3), smoothcombo, FALSE, FALSE, 0);
  gtk_widget_show(smoothcombo);

  quitbutton = gtk_toggle_button_new_with_label("  POWER  ");
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(quitbutton), counter_default_state_power);
  gtk_signal_connect (GTK_OBJECT (quitbutton), "clicked",
		      GTK_SIGNAL_FUNC (power_callback), (gpointer) pdata);
  gtk_box_pack_start(GTK_BOX(box3), quitbutton, FALSE, FALSE, 0);
  gtk_widget_show(quitbutton);
  gtk_tooltips_set_tip (tooltips, quitbutton, "Turn the display ON/OFF", NULL);

  gtk_widget_show(box3);

  /* add a frame */
  frame = gtk_frame_new(NULL);
  gtk_frame_set_label( GTK_FRAME(frame), "VP7 CounterBox (tm)" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);

  /* place main box in frame-container */
  gtk_container_add (GTK_CONTAINER (frame), box2);
  gtk_widget_show(box2);

  mainbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(mainbox), frame, FALSE, FALSE, 0);
  gtk_widget_show(mainbox);

  mytimer = g_timer_new ();
  g_timer_start(mytimer);

  initcounter( pdata,
	       counter_number_of_digits,
	       counter_default_state_power,
	       counter_default_state_mode,
	       counter_default_state_hold
	       );

  return (mainbox);
}

/* ---eof--- */





























