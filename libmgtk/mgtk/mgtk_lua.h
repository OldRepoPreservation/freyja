/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Mgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: mgtk Lua language binding.
 *
 * 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.08.18:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__MGTK_LUA_H_
#define GUARD__MGTK_LUA_H_

#ifdef LUAWRAPPER_FOUND

#   include "lua/Lua.h"

void mgtk_lua_register_functions(const Lua& vm);
/*------------------------------------------------------
 * Pre : lua is a valid VM wrapper.
 * Post: Register mgtk functions for lua use.
 *
 ------------------------------------------------------*/

int mgtk_lua_event(lua_State* s);
/*------------------------------------------------------
 * Pre : 
 * Post: 
 *
 * event_id = mgtk_event( [ symbol | symbol, lua_script | symbol, lua_func, num_args, arg_types ] )
 *
 * -- function do_move(x, y) bound to id:
 * id = mgtk_event( "move", "do_move", 2, "ff" ) 
 *
 ------------------------------------------------------*/


/* Mongoose 2002.01.12, Interfaces for Resource
   must be implmented for each interface ( gtk, win32, etc ) */

int mgtk_lua_rc_window(lua_State *s);
/*------------------------------------------------------
 *  
 * mgtk_window("title" "icon/filename")
 *
 * Signal: None.
 *
 * Post  : 
 *
 ------------------------------------------------------*/

int mgtk_lua_is_null(lua_State *s);
/*------------------------------------------------------
 * Pre : 
 * Post: i = mgtk_is_null( widget )
 * 
 ------------------------------------------------------*/

int mgtk_lua_rc_opengl_canvas(lua_State *s);
// canvas = mgtk_opengl_canvas(width, height)

int mgtk_lua_rc_notebook(lua_State *s);
// notebook = mgtk_notebook( eventId [, width, height] )

int mgtk_lua_rc_tab(lua_State *s);
// mgtk_tab(notebook, label, eventId)

int mgtk_lua_rc_time_slider(lua_State *s);
// slider = mgtk_time_slider( eventId [, width, height] )

int mgtk_lua_rc_range_slider(lua_State *s);
// slider = mgtk_range_slider( eventId [, width, height] )

int mgtk_lua_rc_textbox(lua_State *s);
int mgtk_lua_rc_hsep(lua_State *s);
int mgtk_lua_rc_vsep(lua_State *s);
int mgtk_lua_rc_button(lua_State *s);
int mgtk_lua_rc_colorbutton(lua_State *s);
// clrbtn = mgtk_colorbutton( event )

int mgtk_lua_rc_togglebutton(lua_State *s);

int mgtk_lua_rc_submenu(lua_State *s);
/*------------------------------------------------------
 * 
 * submenu = 
 * mgtk_submenu("label", eventId [, "bind-symbol"] )
 * 
 ------------------------------------------------------*/

int mgtk_lua_rc_menu_separator(lua_State *s);
/*------------------------------------------------------
 * 
 * separator = 
 * mgtk_menu_sep()
 * 
 ------------------------------------------------------*/

int mgtk_lua_rc_menu_item(lua_State *s);
/*------------------------------------------------------
 * Pre : 
 * Post: 
 *
 * menuitem = 
 * mgtk_menu_item("label", eventId [, "icon" | "accel", "icon"])
 * 
 ------------------------------------------------------*/

int mgtk_lua_rc_menu_item_check(lua_State *s);
// menuitem_check =
// mgtk_menu_item( "label" [, event | , event, value ] )

int mgtk_lua_rc_optionmenu(lua_State *s);
int mgtk_lua_optionmenu_set_by_id(lua_State *s);
int mgtk_lua_rc_popup_menu(lua_State *s);
int mgtk_lua_rc_menubar(lua_State *s);

int mgtk_lua_rc_tree(lua_State *s);
// tree = mgtk_tree(label, selection_event, text_event)

int mgtk_lua_rc_hslider(lua_State *s);
// slider = mgtk_hslider(event, min, max)

int mgtk_lua_rc_toolbar(lua_State *s);
/*------------------------------------------------------
 * Pre : Must pass in existing box.
 * Post: 
 *
 * toolbar = 
 * mgtk_toolbar(box [, orientation ] )
 *
 * -- 0 = vertical, 1 = horizatal (default)
 * 
 ------------------------------------------------------*/

int mgtk_lua_rc_toolbar_append(lua_State *s);
/*------------------------------------------------------
 * Pre : Must pass in existing box.
 * Post: 
 *
 * toolbar = 
 * mgtk_toolbar_append(toolbar, item [, item, ..., mode])
 * 
 ------------------------------------------------------*/

int mgtk_lua_rc_toolbar_box(lua_State *s);
/*------------------------------------------------------
 * Pre : Must pass in existing toolbar.
 * Post: Allows you to embed any widget in a toolbar.
 *       Returns a box container to store widgets.
 *
 * box = 
 * mgtk_toolbar_box(toolbar [, AppendMode])
 * 
 ------------------------------------------------------*/

int mgtk_lua_rc_toolbar_button(lua_State *s);

int mgtk_lua_rc_toolbar_menubutton(lua_State *s);

int mgtk_lua_rc_toolbar_togglebutton(lua_State *s);

int mgtk_lua_rc_toolbar_separator(lua_State *s);

int mgtk_lua_rc_toolbar_item(lua_State *s);

int mgtk_lua_rc_color(lua_State *s);
int mgtk_lua_rc_hslider(lua_State *s);
int mgtk_lua_rc_icon(lua_State *s);
int mgtk_lua_rc_dialog(lua_State *s);
int mgtk_lua_rc_query_dialog(lua_State *s);
int mgtk_lua_rc_confirmation_dialog(lua_State *s);
int mgtk_lua_rc_label2(lua_State *s);
int mgtk_lua_rc_fileselection_drop_down_menu(lua_State *s);
int mgtk_lua_rc_filechooserbutton(lua_State *s);
int mgtk_lua_rc_filechoosermenu_item(lua_State *s);

int mgtk_lua_rc_filechoosertoolbar_button(lua_State *s);
// btn = mgtk_filechooserbutton("label", "title", event, "your_mgtk_rc_pattern_callback") 

int mgtk_lua_rc_expander(lua_State *s);
int mgtk_lua_rc_expander_hbox(lua_State *s);

int mgtk_lua_rc_textbox(lua_State* s);


//////////////////////////////////////////////////////////////////////////////
// Containers
////////////////////////////////////////////////////////////////////////////// 

int mgtk_lua_rc_box_pack(lua_State *s);
/*------------------------------------------------------
 *  
 * mgtk_box_pack(box, widget, [ expand, fill, padding ])
 *
 * Signal: None.
 *
 * Post  : Pack a widget into a box and make it visible.
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_hbox(lua_State *s);
/*------------------------------------------------------
 *  
 * mgtk_hbox( [homogeneous, spacing] )
 *
 * Signal: None.
 *
 * Post  : Construct a widget hortizontal container.
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_vbox(lua_State *s);
/*------------------------------------------------------
 *  
 * mgtk_hbox( [homogeneous, spacing] )
 *
 * Signal: None.
 *
 * Post  : Construct a widget vertical container.
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_handlebox(lua_State *s);
/*------------------------------------------------------
 *  
 * mgtk_handlebox( [pos] )
 *
 * Signal: None.
 * Pre   : pos = {0 Top}, {1 Left}, {2 Right}
 * Post  : Construct a detachable widget container.
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_summonbox(lua_State *s);
/*------------------------------------------------------
 *  
 * mgtk_handlebox( symbol )
 *
 * Signal: None.
 * Pre   : Symbol - symbol name of a 'named' widget container.
 * Post  : Reuse a previous 'named' widget container.
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_append_menu(lua_State *s);
/*------------------------------------------------------
 *  
 * mgtk_append_menu( menu, menuitem )
 *
 ------------------------------------------------------*/



//////////////////////////////////////////////////////////////////////////////
// Event widgets
////////////////////////////////////////////////////////////////////////////// 

int mgtk_lua_rc_spinbutton_uint(lua_State *s);
/*------------------------------------------------------
 *  
 * spinbutton = 
 * mgtk_spinbutton_uint(eventId, initialValue, min, max [, step, page, pageSize])
 *
 * Signal: mgtk_event_spinbutton_uint
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_spinbutton_int(lua_State *s);
/*------------------------------------------------------
 *  
 * spinbutton = 
 * mgtk_spinbutton_int(eventId, initialValue, min, max [, step, page, pageSize])
 *
 * Signal: mgtk_event_spinbutton_int
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_spinbutton_float(lua_State *s);
/*------------------------------------------------------
 *  
 * spinbutton = 
 * mgtk_spinbutton_float(eventId, initialValue, min, max, digits [, step, page, pageSize])
 *
 * Signal: mgtk_event_spinbutton_float
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_statusbar(lua_State *s);

int mgtk_lua_rc_textview(lua_State *s);
/*------------------------------------------------------
 *  
 * textview = 
 * mgtk_textview( "text" )
 *
 * Signal: 
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_label(lua_State *s);
/*------------------------------------------------------
 *  
 * label = 
 * mgtk_label( "text", x_align, y_align )
 *
 * Signal: 
 *
 ------------------------------------------------------*/

int mgtk_lua_dialog(lua_State *s);
int mgtk_lua_confirmation_dialog(lua_State *s);
int mgtk_lua_query_dialog(lua_State *s);

int mgtk_lua_boolean_set(lua_State *s);
// mgtk_boolean_set( "boolean_symbol", boolean_value )

int mgtk_lua_color_set(lua_State *s);
// mgtk_color_set( "color_symbol", redf, greenf, bulef, alphaf ) 

int mgtk_lua_float_set(lua_State *s);
// mgtk_float_set( "float_symbol", float_value )

int mgtk_lua_integer_set(lua_State *s);
// mgtk_integer_set( "integer_symbol", integer_value )

int mgtk_lua_string_set(lua_State *s);
// mgtk_string_set( "string_symbol", "string_value" )

int mgtk_lua_window_move(lua_State* s);

#endif // LUAWRAPPER_FOUND

#endif // GUARD__MGTK_LUA_H_
