/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL See file COPYING, also (C) 2000, 2001 Mongoose
 * Comments: 
 *
 *-- TODO ---------------------------------------------------
 *
 * Merge this into unified Freyja class then
 * move gtk dependent code out into FreyjaGnome
 *
 *-- History ------------------------------------------------- 
 *
 * 2000-09-16:
 * Mongoose - Created
 =================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_GNOME_RESOURCE_H_
#define GUARD__FREYJA_MONGOOSE_GNOME_RESOURCE_H_

enum
{
   NAME_COLUMN = 0,
   ID_COLUMN,
   N_COLUMNS
};


GtkWidget *getGtkMainWindow();
GtkWidget *getGtkGLAreaWidget();
GtkWidget *getGtkStatusBarWidget();
GtkWidget *getGtkFileSelectionWidget();

void application_window_fullscreen();
void application_window_unfullscreen();
void application_window_move(int x, int y);
void application_window_resize(int width, int height);
void application_window_role(char *role);

void resource_rebuild_skeleton_treeview(GtkTreeModel *model);

#endif
