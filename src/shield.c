/**************************************************************************/
/*                                                                        */
/*   shield.c - Shield module to the VP7 Way Kool Project                 */
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gtk/gtk.h>
#include "source.h"
#include "gfx.h"
#include "shield.h"

gfloat dist;
gfloat rhoal = 2.7;  /* g/cm**3 */
gfloat rhopb = 11.3; /* g/cm**3 */
GtkWidget *alspin,*pbspin;  /* need them globally :-( */
GtkAdjustment *dadj;

static walltype wall;       /* alias local wall, private */



void set_distance (GtkAdjustment *dadj, GtkWidget *widget)
{
  if (dadj->value < (wall.al+wall.pb)) /* can't come closer than the */
    dadj->value = wall.al+wall.pb;     /* thickness of shields       */
  gtk_signal_emit_by_name (GTK_OBJECT (dadj), "changed");
}


/* Yes, yes, I know these caalbacks can be done much nicer */
/* on the other hand...                                    */
void toggle_pa( GtkWidget *widget)
{
  wall.pa = !(wall.pa);
  set_paper_att(wall.pa);   /* this one for the graphics */
}

void toggle_al( GtkWidget *widget)
{
  wall.al = !(wall.al);
  set_al_att(wall.al);
  if (wall.al)              /* get spinbutton value if turned on */
    wall.al = gtk_spin_button_get_value_as_int( GTK_SPIN_BUTTON (alspin) );
  set_distance(dadj,NULL);
}

void toggle_pb( GtkWidget *widget)
{
  wall.pb = !(wall.pb);
  set_pb_att(wall.pb);
  if (wall.pb)              /* get spinbutton value if turned on */
    wall.pb = gtk_spin_button_get_value_as_int( GTK_SPIN_BUTTON (pbspin) );
  set_distance(dadj,NULL);
}

void set_al_thickness (GtkAdjustment *adj, GtkWidget *widget)
{
  if (wall.al) {
    wall.al = adj->value;
    set_distance(dadj,NULL);
  }
}

void set_pb_thickness (GtkAdjustment *adj, GtkWidget *widget)
{
  if (wall.pb) {
    wall.pb = adj->value;
    set_distance(dadj,NULL);
  }
}

GtkWidget *init_slide(GtkWidget *window)
{
  GtkWidget *hbar,*frame;

  dadj = (GtkAdjustment *) gtk_adjustment_new(1,1,500,1,10,0);
  gtk_signal_connect (GTK_OBJECT (dadj), "value_changed",
		      GTK_SIGNAL_FUNC (set_distance), NULL);
  hbar = gtk_hscale_new( dadj );
  gtk_scale_set_draw_value( GTK_SCALE (hbar), TRUE);
  gtk_scale_set_digits( GTK_SCALE (hbar), 0);
  gtk_scale_set_value_pos( GTK_SCALE (hbar), GTK_POS_LEFT);
  /* make frame around slidebar */
  frame = gtk_frame_new( NULL );
  gtk_frame_set_label ( GTK_FRAME (frame)," Source/Detector Distance (mm) ");
  gtk_frame_set_label_align ( GTK_FRAME (frame), 0.0,0.0);
  gtk_frame_set_shadow_type ( GTK_FRAME (frame), GTK_SHADOW_ETCHED_OUT);

  gtk_container_add (GTK_CONTAINER (frame), hbar);
  gtk_widget_show_all( frame );

  return frame;
}

GtkWidget *init_shields(GtkWidget *window)
{
  GtkWidget *table,*frame;
  GtkWidget *pabutton,*albutton,*pbbutton; /* check buttons */
  GtkStyle *style;
  GtkAdjustment *adjal,*adjpb;             /* for spinbuttons */           
  GtkWidget *label;

  style = gtk_widget_get_style(window);
  printf("Setting up shields...");

  /* Table to contain everything */
  table = gtk_table_new (3,3,TRUE);

  /* paper toggle button */
  pabutton = gtk_check_button_new_with_label(" Paper");
  gtk_signal_connect (GTK_OBJECT (pabutton), "clicked",
		      GTK_SIGNAL_FUNC (toggle_pa), NULL);
  gtk_table_attach_defaults (GTK_TABLE (table),pabutton,0,2,0,1);

  /* aluminium toggle button */
  albutton = gtk_check_button_new_with_label(" Al");
  gtk_signal_connect (GTK_OBJECT (albutton), "clicked",
		      GTK_SIGNAL_FUNC (toggle_al), NULL);
  adjal = (GtkAdjustment *) gtk_adjustment_new(1,1,100,1,10,0);
  gtk_signal_connect (GTK_OBJECT (adjal), "value_changed",
		      GTK_SIGNAL_FUNC (set_al_thickness), NULL);
  alspin = gtk_spin_button_new (adjal,0,0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (alspin),FALSE);
  gtk_spin_button_set_shadow_type (GTK_SPIN_BUTTON (alspin),GTK_SHADOW_OUT);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (alspin), TRUE ); /* safety... */
  label = gtk_label_new (" mm");
  gtk_table_attach_defaults (GTK_TABLE (table), (albutton),0,1,1,2);
  gtk_table_attach_defaults (GTK_TABLE (table), (alspin),1,2,1,2);
  gtk_table_attach_defaults (GTK_TABLE (table), (label),2,3,1,2);
  set_al_thickness (adjal,NULL);
  /* lead toggle button */
  pbbutton = gtk_check_button_new_with_label(" Pb");
  gtk_signal_connect (GTK_OBJECT (pbbutton), "clicked",
		      GTK_SIGNAL_FUNC (toggle_pb), NULL);
  adjpb = (GtkAdjustment *) gtk_adjustment_new(1,1,100,1,10,0);
  gtk_signal_connect (GTK_OBJECT (adjpb), "value_changed",
		      GTK_SIGNAL_FUNC (set_pb_thickness), NULL);
  pbspin = gtk_spin_button_new (adjpb,0,0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (pbspin),FALSE);
  gtk_spin_button_set_shadow_type (GTK_SPIN_BUTTON (pbspin),GTK_SHADOW_OUT);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (pbspin), TRUE ); /* safety... */
  label = gtk_label_new (" mm");
  gtk_table_attach_defaults (GTK_TABLE (table), (pbbutton),0,1,2,3);
  gtk_table_attach_defaults (GTK_TABLE (table), (pbspin),1,2,2,3);
  gtk_table_attach_defaults (GTK_TABLE (table), (label),2,3,2,3);
  set_pb_thickness (adjpb,NULL);
  /* make frame around box */
  frame = gtk_frame_new( NULL );
  gtk_frame_set_label ( GTK_FRAME (frame)," Shield Remote Control ");
  gtk_frame_set_label_align ( GTK_FRAME (frame), 0.0,0.0);
  gtk_frame_set_shadow_type ( GTK_FRAME (frame), GTK_SHADOW_ETCHED_OUT);

  gtk_container_add (GTK_CONTAINER (frame), table);
  gtk_widget_show_all (frame);

  return (frame);
}

/* Return range (mm) of alpha particles of energy E in air */
/* Empirical formula (see documentation)                   */
gfloat arir(gfloat E)
{
	if ( E > 0 )
		return -0.2643*E+0.96905*pow(E,2)+6.4024;
	else
		return 0;
}

/* Return range (mm) of beta particles of energy E in Al */
/* Empirical formula (see documentation)                 */
gfloat brial(gfloat E)
{
	if ( E > 0 )
		return 0.114209*pow(E,2)+1.359*E-0.150313;
	else
		return 0;
}


/* Returns half thickness (g/cm**2) of gamma rays in Pb   */
/* Empirical formula (see documentation)                  */
/* below 100keV the half thicknes is discontinous, that's */
/* why the strange case switch has been used. Above 5MeV  */
/* the formula might be very wrong (the data was only     */
/* up to 5MeV).                                           */
gfloat gamma_half_thick_pb(gfloat E)
{
  int TE;

  if (E>0.09) {
    return -1.36533*pow(E,2)+10.649*E-1.03084;
  }
  else {
    TE = (int)(100*E);
    switch (TE) {
    case 1:
      return 0.003;
    case 2:
      return 0.01;
    case 3:
      return 0.038;
    case 4:
      return 0.06;
    case 5:
      return 0.11;
    case 6:
      return 0.17;
    case 7:
      return 0.255;
    case 8:
      return 0.4;
    case 9:
      return 0.3;
    default:
      return 0.03; /* well ... */
    }
  }
    
}

/* Returns half thickness (g/cm**2) of gamma rays in Al    */
/* Empirical formula (see documentation)                   */
/* The data was measured in the range og 10keV and 5MeV    */
/* and outside this range the returned values are expected */
/* to be incorrect. Particularly, above 5MeV the curve is  */
/* expected to drop off.                                   */
gfloat gamma_half_thick_al(gfloat E)
{
	if (E<0.035)
		return 30000.0*pow(E,3.05);
	else if (E<0.082)
		return 160.0*pow(E,1.5);
	else
		return 12.0*pow(E,0.45);
}


/* New attenuation engine                                */
/* if wall.pa : set alpha=0                              */
/* if wall.al : set alpha=0                              */
/*              check beta_range_in_al                   */
/*              att. gamma                               */
/* if wall.pb : set alpha=0                              */
void attenuate(henfald_struct *henf)
{
  int i;
  int air;

  air = dadj->value-wall.al-wall.pb;
  /* alpha range in air */
  for (i=0; i<3; i++)
    henf->alpha[i].N = 
      (air<arir(henf->alpha[i].energy)) ? henf->alpha[i].N : 0;  
  /* alpha can't penetrate even paper */
  if ( wall.pa )
    for (i=0; i<3; i++)
      henf->alpha[i].N = 0;

  if (wall.al ) {
    /* alpha */
    for (i=0; i<3; i++)
      henf->alpha[i].N = 0;
    /* beta */
    for (i=0; i<3; i++)
	    henf->beta[i].N = 
		    (wall.al<brial(henf->beta[i].energy)) ? henf->beta[i].N : 0;
    /* gamma */
    for (i=0; i<3; i++)
	    /* Calculate only if we have a source with E > 0 */
	    /* Otherwise we end up with 0*inf decays which   */
	    /* give -MAXINT counts in the counter.            */
	    if ( henf->gamma[i].energy > 0 )
		    henf->gamma[i].N = henf->gamma[i].N*
			    exp(-log(2.0)*rhoal*gtk_spin_button_get_value_as_int( GTK_SPIN_BUTTON (alspin) )/
				(10.0*gamma_half_thick_al(henf->gamma[i].energy)));
  }
  if ( wall.pb ) {
    /* alpha */
    for (i=0; i<3; i++)
      henf->alpha[i].N = 0;
    /* beta */
    for (i=0; i<3; i++)
      henf->beta[i].N = 0;
    /* gamma */
    for (i=0; i<3; i++)
      henf->gamma[i].N = henf->gamma[i].N*
	exp(-log(2.0)*rhopb*gtk_spin_button_get_value_as_int( GTK_SPIN_BUTTON (pbspin) )/
	    (10.0*gamma_half_thick_pb(henf->gamma[i].energy)));    
  }

}


