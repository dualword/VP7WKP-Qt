/**************************************************************************/
/*                                                                        */
/*   source.c - Source module to the VP7 Way Kool Project                 */
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
#include <math.h>
#include <string.h>
#include "source.h"
#include "gfx.h"
#include "toolbox.h"
#include "miniterm.h"

/************************************************************/
/*       Variables and functions related to source          */
/************************************************************/

#define LOG2 0.6931471806

/* the active sources */
sourcetype sourcearray[MAXSOURCE];         

/* number of nuclei to begin with = mass/(amu*1.6605655e-24) */
double Ntilbage[MAXSOURCE];


extern gint close_application(GtkWidget *, GdkEvent *, gpointer data );


/* load a source data from a sourcefile */
int loadsource(char *filename,int pos)
{
	FILE *src;
	char *line;
	int i;

	if ((src=fopen(filename,"r")) == NULL) {
		printf("Unable to find!!\n");
		return -1;
	} else {
		sourcearray[pos].id = pos;
		sourcearray[pos].filename = malloc(strlen(filename)+1);
		sourcearray[pos].filename = strcpy(sourcearray[pos].filename,filename);
		/* the name of the source */
		line = malloc(50);
		line = fgets(line,50,src);
		line[strlen(line)-1] = '\0';  /* remove EOL */
		sourcearray[pos].name = malloc(strlen(line)+1);
		sourcearray[pos].name = strcpy(sourcearray[pos].name,line);
		/* get the pixmaps */
		line = fgets(line,32,src);
		line[strlen(line)-1] = '\0';  /* remove EOL */
		sourcearray[pos].onname = malloc(strlen(line)+1);
		sourcearray[pos].onname = strcpy(sourcearray[pos].onname,line);
		line = fgets(line,32,src);
		line[strlen(line)-1] = '\0';  /* remove EOL */
		sourcearray[pos].ofname = malloc(strlen(line)+1);
		sourcearray[pos].ofname = strcpy(sourcearray[pos].ofname,line);
		/* get the atomic mass, weight and half life */
		line = fgets(line,50,src);
		line[strlen(line)-1] = '\0';  /* remove EOL */
		sscanf(line,"%g %g %g",&sourcearray[pos].amu,
		       &sourcearray[pos].mass,
		       &sourcearray[pos].T);
		/* get alpha modes */
		for (i=0; i<3; i++) {
			sourcearray[pos].alpha[i].N = 0;
			line = fgets(line,50,src);
			line[strlen(line)-1] = '\0';  /* remove EOL */
			sscanf(line,"%g %g",&sourcearray[pos].alpha[i].energy,
			       &sourcearray[pos].alpha[i].frac);
		}
		/* load beta modes */
		for (i=0; i<3; i++) {
			sourcearray[pos].beta[i].N = 0;
			line = fgets(line,50,src);
			line[strlen(line)-1] = '\0';  /* remove EOL */
			sscanf(line,"%g %g",&sourcearray[pos].beta[i].energy,
			       &sourcearray[pos].beta[i].frac);
		}
		/* load gamma modes */
		for (i=0; i<3; i++) {
			sourcearray[pos].gamma[i].N = 0;
			line = fgets(line,50,src);
			line[strlen(line)-1] = '\0';  /* remove EOL */
			sscanf(line,"%g %g",&sourcearray[pos].gamma[i].energy,
			       &sourcearray[pos].gamma[i].frac);
		}
		/* load additional info */
		for (i=0; (i<MAXINFO) && ((line=fgets(line,50,src))!=NULL); i++) {
			/* remove EOL */
			line[strlen(line)-1] = '\0';
			/* has info storage been used? */
			if (sourcearray[pos].info[i] != NULL)
				free(sourcearray[pos].info[i]);
			sourcearray[pos].info[i] = malloc(strlen(line)+1);
			sourcearray[pos].info[i] = strcpy(sourcearray[pos].info[i],line);
		}
		
		fclose(src);
		free(line);
		return 0;
	}
}


void initsource()
{
	int pos,i;
	FILE *cfg;        /* configfile */
	char *fname;
	
	if ((cfg = fopen("./vp7wkp.cfg","r")) == NULL) {
		printf("An error ocurred opening vp7wkp.cfg -> empty.src\n");
		/* load empty in every place */
		for ( pos=0; pos<MAXSOURCE; pos++) {
			i = loadsource("./sources/empty.src",pos);
		}
	} else {
		fname = malloc(32);
		pos = 0;
		while ((( fname=fgets(fname,32,cfg)) != NULL ) && ( pos < MAXSOURCE )) {
			/* remove EOL */
			if (fname != NULL) {
				fname[strlen(fname)-1] = '\0';
				printf("   Trying %s to position %d...",fname,pos);
				i = loadsource(fname,pos);
				printf("done\n");
				Ntilbage[pos] = sourcearray[pos].mass/(sourcearray[pos].amu*1.6606e-24);
				pos++;
			}
		}
		free(fname);
		fclose(cfg);
		/* load empty.src if there are unused places */
		while (pos<MAXSOURCE) {
			printf("   Trying ./sources/empty.src to position %d...",pos);
			i = loadsource("sources/empty.src",pos);
			printf("done\n");
			Ntilbage[pos] = 0;
			pos++;
		}      
	}
	printf("Size of one source record: %d\n",sizeof(sourcetype));
}


double gauss(double mean, double var)
{
	double x, s;
	int i;
	
	s = 0.0;
	for (i = 0; i<12; i++) {
		/* get random number between 0..1 */
		x = (double)random()/((double)RAND_MAX + 1.0);
		s += x;
	}
	s -= 6.0;
	
	return (mean + s*var);
}


/* Calculate the number of decayed nuclei given source and time */
henfald_struct dodecay(double dt, int n)
{
	henfald_struct decay = {{ {0,0,0},{0,0,0},{0,0,0} },
				{ {0,0,0},{0,0,0},{0,0,0} },
				{ {0,0,0},{0,0,0},{0,0,0} }};
	
	henfald_struct error = {{ {-1,-1,-1},{-1,-1,-1},{-1,-1,-1} },
				{ {-1,-1,-1},{-1,-1,-1},{-1,-1,-1} },
				{ {-1,-1,-1},{-1,-1,-1},{-1,-1,-1} }};
	double nt,dn;
	int i;
	double d;
	
	
	if ( ( src < 0 ) || (src > MAXSOURCE) )
		return error;
	
	if ( sourcearray[src].T > 0 ) {
		d = dt*LOG2/sourcearray[src].T;
		nt = Ntilbage[src]*(-expm1(-d));
		dn = floor(gauss(nt, sqrt(nt)));
	} else
		dn = 0;
	
	if (dn < 0.0) dn = 0.0;
	
	Ntilbage[src] -= dn;
	
	for (i=0; i<3; i++) {
		decay.alpha[i].N = dn*sourcearray[src].alpha[i].frac;
		decay.alpha[i].energy = sourcearray[src].alpha[i].energy;
		decay.beta[i].N = dn*sourcearray[src].beta[i].frac;
		decay.beta[i].energy = sourcearray[src].beta[i].energy;
		decay.gamma[i].N = dn*sourcearray[src].gamma[i].frac;
		decay.gamma[i].energy = sourcearray[src].gamma[i].energy;
	}
	
	return decay;  
}


/*******************************************************************/
/* Variables and functions related to the graphic source-selector  */
/*******************************************************************/

GtkWidget *vpixmaps[MAXSOURCE];   /* drawable pixmaps to display */
GdkPixmap *ofpixmap[MAXSOURCE];   /* OFF style GdkPixmaps        */
GdkBitmap *ofmask[MAXSOURCE];     /* masks for off-pixmaps       */
GdkPixmap *onpixmap[MAXSOURCE];   /* ON style GdkPixmaps         */
GdkBitmap *onmask[MAXSOURCE];     /* masks for on-pixmaps        */
GtkWidget *ebox[MAXSOURCE];       /* Eventboxes to trap signals  */
/* Actually, the code has been rewritten to use buttons instead  */
/* of event-boxes, but the variable names have been kept.        */

/* Widgets related to Exit-button */
GdkPixmap *exitpixmap; 
GdkBitmap *exitmask;
GtkWidget *exitpixmapwid,*exitebox;

/* widgets related to toolbox */
GdkPixmap *toolpixmap;
GdkBitmap *toolmask;
GtkWidget *toolpixmapwid,*toolbutton;
GtkTooltips *toolboxtips;

void setsource(int n)
{
	gchar *text;

	text = g_malloc(21);
	g_snprintf( text, 21, "Found at pos. %d", n);
	miniterm_print( text );
	miniterm_linefeed();
	g_free( text );

	gtk_pixmap_set( (GtkPixmap *)vpixmaps[src], ofpixmap[src], ofmask[src]);
	if ( (n<0) || (n>=MAXSOURCE) )
		src = 0;
	else
		src = n;
	gtk_pixmap_set( (GtkPixmap *)vpixmaps[src], onpixmap[src], onmask[src]);
}


void set(GtkWidget *widget)
{
	int *id;
	gchar *text;
	
	/* get the ID attached to GTK_OBJECT */
	id = gtk_object_get_data(GTK_OBJECT(widget),"ID");
	/* set the source */
	/*printf( "Trying %s... ",sourcearray[*id].name );*/
	text = g_malloc(50);
	g_snprintf( text, 50, "Trying %s... ", sourcearray[*id].name );
	miniterm_print( text );
	g_free(text);
	/*setsource(*id);*/
	/*set_active_button(*id);*/
	setsource(*id);
	/* update Lab-pixmap */
	if ( !strcmp(sourcearray[*id].name,"Empty") )
		set_source(0);
	else
		set_source(1);
}

GtkWidget *create_skab(GtkWidget *window)
{
	gint i;
	GtkWidget *skab,*frame;
	GtkStyle *style;
	gchar *text;

	miniterm_print("Integrity check of radioactive materials:\n");
	style = gtk_widget_get_style( window );
	/* create box */
	skab = gtk_hbox_new(FALSE,2);
	
	/* load pixmaps from files & place in eventboxes */
	for (i=0; i<MAXSOURCE; i++) {
		if ((ofpixmap[i] = gdk_pixmap_create_from_xpm(window->window,&ofmask[i],
							      &style->bg[GTK_STATE_NORMAL],
							      sourcearray[i].ofname))
		    == NULL)
			ofpixmap[i] = gdk_pixmap_create_from_xpm(window->window,&ofmask[i],
								 &style->bg[GTK_STATE_NORMAL],
								 "./pixmaps/radiof.xpm");
		if ((onpixmap[i] = gdk_pixmap_create_from_xpm(window->window,&onmask[i],
							      &style->bg[GTK_STATE_NORMAL],
							      sourcearray[i].onname))
		    == NULL)
			onpixmap[i] = gdk_pixmap_create_from_xpm(window->window,&onmask[i],
								 &style->bg[GTK_STATE_NORMAL],
								 "./pixmaps/radion.xpm"); 
		vpixmaps[i] = gtk_pixmap_new(ofpixmap[i],ofmask[i]);
		/*gtk_widget_show(vpixmaps[i]);*/
   
		/* sourcebuttons */
		ebox[i] = gtk_button_new();
		gtk_box_pack_start( GTK_BOX (skab), ebox[i], FALSE, FALSE, 0);
		/*gtk_widget_show(ebox[i]);*/
		gtk_container_add( GTK_CONTAINER(ebox[i]), vpixmaps[i]);
		/* Attach an ID to each eventbox */
		gtk_object_set_data(GTK_OBJECT(ebox[i]),"ID",(gpointer)&sourcearray[i].id);
		/* events */
		gtk_widget_set_events( ebox[i], GDK_BUTTON_PRESS_MASK );
		gtk_signal_connect( GTK_OBJECT(ebox[i]), "clicked",
				    GTK_SIGNAL_FUNC(set), NULL );
		text = g_malloc( 40 );
		g_snprintf( text, 40, "   %s loaded", sourcearray[i].name);
		miniterm_print( text );
		miniterm_linefeed();
		g_free( text );
	}
	
	/* quit-button */
	exitpixmap = gdk_pixmap_create_from_xpm( window->window,  &exitmask,
						 &style->bg[GTK_STATE_NORMAL],
						 "./pixmaps/exit.xpm");
	exitpixmapwid = gtk_pixmap_new( exitpixmap, exitmask );
	/*gtk_widget_show(exitpixmapwid);*/
	exitebox = gtk_event_box_new();
	gtk_box_pack_end( GTK_BOX (skab), exitebox, FALSE, FALSE, 20);
	/*gtk_widget_show(exitebox);*/
	gtk_container_add( GTK_CONTAINER(exitebox), exitpixmapwid);
	gtk_signal_connect (GTK_OBJECT(exitebox), "button_press_event", 
			    GTK_SIGNAL_FUNC(close_application),NULL);
	
	/* Tollbox button */
	toolpixmap = gdk_pixmap_create_from_xpm( window->window, &toolmask,
						 &style->bg[GTK_STATE_NORMAL],
						 "./pixmaps/toolbox.xpm");
	toolpixmapwid = gtk_pixmap_new( toolpixmap, toolmask );
	/*gtk_widget_show(toolpixmapwid);*/
	toolbutton = gtk_button_new();
	gtk_box_pack_end(GTK_BOX(skab), toolbutton, FALSE, FALSE,0);
	/*gtk_widget_show(toolbutton);*/
	gtk_container_add( GTK_CONTAINER(toolbutton), toolpixmapwid);
/*	gtk_signal_connect(GTK_OBJECT(toolbutton),"clicked",
	GTK_SIGNAL_FUNC(setupsource),NULL);*/
	gtk_signal_connect( GTK_OBJECT(toolbutton), "clicked",
			    GTK_SIGNAL_FUNC(miniterm_testcolors),
			    NULL );
	toolboxtips = gtk_tooltips_new();
	gtk_tooltips_set_tip( toolboxtips,toolbutton,"Radioactive ToolBox",NULL);
	
	/* make frame around all the buttons */
	frame = gtk_frame_new(NULL);
	gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
	gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
	/*gtk_widget_show(frame);*/
	
	gtk_container_add (GTK_CONTAINER (frame), skab);
	/*gtk_widget_show(skab);*/
	
	gtk_widget_show_all( frame );
	
	setsource(0);
	
	return frame;
}
