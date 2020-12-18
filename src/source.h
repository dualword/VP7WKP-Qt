/**************************************************************************/
/*                                                                        */
/*   source.h  - Source module header to the VP7 Way Kool Project         */
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

#define MAXSOURCE 10	  /* Max number of sources hold   */
#define MAXINFO 20        /* Max number of info-lines     */

typedef struct {
  int   N;                /* Calculated froom "frac" */  
  float energy;
  float frac;
} radiation_struct;

typedef struct {
  radiation_struct alpha[3];
  radiation_struct beta[3];
  radiation_struct gamma[3];
} henfald_struct;

typedef struct {
  int   id;         /* ID used to identify GtkObject */
  char  *filename;
  char  *name;
  char  *onname;
  char  *ofname;
  float amu;        /* atomic weight */
  float mass;       /* gram */
  float T;          /* half life (sec) */
  radiation_struct alpha[3];
  radiation_struct beta[3];
  radiation_struct gamma[3];
  char *info[MAXINFO];     /* additional info */
} sourcetype;



henfald_struct dodecay(double deltat, int source);
void initsource(void);

gint src;
void setsource(int n);
GtkWidget *create_skab(GtkWidget *window);

