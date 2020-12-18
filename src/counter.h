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
/* counter.h v.0.4.2*/
gboolean counter_update(guint32 new_counts);
GtkWidget *counter_create (GtkWidget *window,
			   const guint8 counter_number_of_digits,
			   const gboolean counter_default_state_power,
			   const gboolean counter_default_state_mode,
			   const gboolean counter_default_state_hold
			   );
/* --- eof --- */











