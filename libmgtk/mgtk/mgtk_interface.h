/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (c) 2000-2004 Mongoose
 * Comments: This is the gtk+ wrapper
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.10.27:
 * Mongoose - Major API clean up, documentation update
 *
 * 2004.06.01:
 * Mongoose - Last of freyja use removed, project desn changed
 *
 * 2004.04.11:
 * Mongoose - Removed GNOME support
 *
 * 2000.09.16:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_FREYJA_INTERFACE_H_
#define GUARD__FREYJA_MONGOOSE_FREYJA_INTERFACE_H_

#include <gtk/gtk.h>

GtkWidget *mgtk_create_color_picker_dialog(char *title, void *event_func);

GtkWidget *mgtk_create_window(char *title, char *wmclass, char *icon_filename);

GtkWidget *mgtk_create_text_entry(GtkWidget *box);

GtkWidget *mgtk_create_icon(const char *icon_filename, GtkIconSize icon_size);

GtkWidget *mgtk_create_vbox(GtkWidget *box, char *name, 
							bool homogeneous, int spacing,
							bool expand, bool fill, int pading);

GtkWidget *mgtk_create_hbox(GtkWidget *box, char *name, 
							bool homogeneous, int spacing,
							bool expand, bool fill, int pading);

GtkWidget *mgtk_create_spinbutton(GtkWidget *master, char *name, 
								  float val, float min, float max);

GtkWidget *mgtk_create_spinbutton2(GtkWidget *master, char *name, 
								   float val, float min, float max, float step,
								   float page, float page_sz, int digits);

GtkWidget *mgtk_create_label(GtkWidget *master, const char *name, 
							 const char *text, float x_align, float y_align);

GtkWidget *mgtk_create_tab(GtkWidget *notebook,  const char *name, 
						   GtkWidget *tab_contents, const char *label_text,
						   int tab_num);

GtkWidget *mgtk_create_notebook(GtkWidget *app);

GtkWidget *mgtk_create_toolbar(GtkWidget *box);

GtkWidget *mgtk_create_toolbar_toogle_button(GtkWidget *toolbar,  bool toggled,
											 const char *icon, const char *label,
											 const char *help, 
											 void *event_func, int event_cmd);

GtkWidget *mgtk_create_toolbar_button(GtkWidget *toolbar, int is_menu,
									  const char *icon, const char *label, const char *help, 
									  void *event_func, int event_cmd);

GtkWidget *mgtk_create_color_button(void *func, int id);

GdkPixbuf* mgtk_create_pixbuf(const char* icon_filename);

void mgtk_destroy_window(GtkWidget *widget);

void mgtk_accel_support(GtkWidget *item, const char* accel);

#endif
