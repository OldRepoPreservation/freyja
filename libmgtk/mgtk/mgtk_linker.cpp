/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (c) 2000-2004 Mongoose
 * Comments: This is the place where callbacks are 'linked' 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- TODO ---------------------------------------------------- 
 *
 * This may be replaced with python or mlisp glue later, but for now it's
 * proven and stable code that's easy to use cross platform / languages.
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.04.01:
 * Mongoose - Moved callback linker into it's own file, minor API clean up.
 *
 * 2004.10.27:
 * Mongoose - Major API clean up, documentation update
 *
 * 2000.09.16:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "mgtk_events.h" // Only for mgtk_print
#include "ResourceEvent.h" // Old observer / listener system.
#include "mgtk_linker.h" 


void (*mgtk_link_callback_get_image_data_rgb24)(const char *, unsigned char **, int *, int *) = NULL;
void (*mgtk_link_handle_color)(int, float, float, float, float) = NULL;
void (*mgtk_link_handle_application_window_close)() = NULL;
void (*mgtk_link_handle_command)(int) = NULL;
void (*mgtk_link_handle_event1u)(int, unsigned int) = NULL;
void (*mgtk_link_handle_event2u)(int, unsigned int, unsigned int) = NULL;
void (*mgtk_link_handle_event1f)(int, float) = NULL;
void (*mgtk_link_handle_file_dialog_selection)(int, char *) = NULL;
void (*mgtk_link_handle_gldisplay)() = NULL;
void (*mgtk_link_handle_glresize)(unsigned int, unsigned int) = NULL;
void (*mgtk_link_handle_key_press)(int, int) = NULL;
void (*mgtk_link_handle_motion)( mgtk_mouse_event_t* ) = NULL;
void (*mgtk_link_handle_mouse)(int, int, int, int, int) = NULL;
void (*mgtk_link_handle_resource_start)() = NULL;
void (*mgtk_link_handle_text_array)(int, unsigned int, char **) = NULL;
void (*mgtk_link_handle_text)(int, char *) = NULL;
void (*mgtk_link_print)(const char*, ...) = NULL;
void (*mgtk_link_get_pixmap_filename)(char *, unsigned int, char *) = NULL;
const char *(*mgtk_link_rc_map)(const char* ) = NULL;
const char *(*mgtk_link_get_resource_path)() = NULL;


int mgtk_link_import(const char *symbol, void *func)
{
	if (strncmp("mgtk_callback_get_image_data_rgb24", symbol, 41) == 0)
	{
		mgtk_link_callback_get_image_data_rgb24 = (void (*)(const char *, unsigned char **, int *, int *))func;
	}
	else if (strncmp("mgtk_get_resource_path", symbol, 29) == 0)
	{
		mgtk_link_get_resource_path = (const char *(*)())func;
	}
	else if (strncmp("mgtk_handle_color", symbol, 26) == 0)
	{
		mgtk_link_handle_color = (void (*)(int, float, float, float, float))func;
	}
	else if (strncmp("mgtk_handle_application_window_close", symbol, 26) == 0)
	{
		mgtk_link_handle_application_window_close = (void (*)())func;
	}
	else if (strncmp("mgtk_handle_command", symbol, 26) == 0)
	{
		mgtk_link_handle_command = (void (*)(int))func;
	}
	else if (strncmp("mgtk_handle_event1u", symbol, 26) == 0)
	{
		mgtk_link_handle_event1u = (void (*)(int, unsigned int))func;
	}
	else if (strncmp("mgtk_handle_event2u", symbol, 26) == 0)
	{
		mgtk_link_handle_event2u = (void (*)(int, unsigned int, unsigned int))func;
	}
	else if (strncmp("mgtk_handle_event1f", symbol, 26) == 0)
	{
		mgtk_link_handle_event1f = (void (*)(int, float))func;
	}
	else if (strncmp("mgtk_handle_file_dialog_selection", symbol, 40) == 0)
	{
		mgtk_link_handle_file_dialog_selection = (void (*)(int, char*))func;
	}
	else if (strncmp("mgtk_handle_gldisplay", symbol, 28) == 0)
	{
		mgtk_link_handle_gldisplay = (void (*)())func;
	}
	else if (strncmp("mgtk_handle_glresize", symbol, 27) == 0)
	{
		mgtk_link_handle_glresize = (void (*)(unsigned int,unsigned int))func;
	}
	else if (strncmp("mgtk_handle_key_press", symbol, 28) == 0)
	{
		mgtk_link_handle_key_press = (void (*)(int,int))func;
	}
	else if (strncmp("mgtk_handle_motion", symbol, 25) == 0)
	{
		mgtk_link_handle_motion = (void (*)(mgtk_mouse_event_t*))func;
	}
	else if (strncmp("mgtk_handle_mouse", symbol, 24) == 0)
	{
		mgtk_link_handle_mouse = (void (*)(int, int, int, int, int))func;
	}
	else if (strncmp("mgtk_handle_resource_start", symbol, 33) == 0)
	{
		mgtk_link_handle_resource_start = (void (*)())func;
	}
	else if (strncmp("mgtk_handle_text_array", symbol, 23) == 0)
	{
		mgtk_link_handle_text_array = (void (*)(int, unsigned int, char **))func;
	}
	else if (strncmp("mgtk_handle_text", symbol, 17) == 0)
	{
		mgtk_link_handle_text = (void (*)(int, char*))func;
	}
	else if (strncmp("mgtk_print", symbol, 17) == 0)
	{
		mgtk_link_print = (void (*)(const char*, ...))func;
	}
	else if (strncmp("mgtk_get_pixmap_filename", symbol, 31) == 0)
	{
		mgtk_link_get_pixmap_filename = (void (*)(char *, unsigned int, char *))func;
	}
	else if (strncmp("mgtk_rc_map", symbol, 18) == 0)
	{
		mgtk_link_rc_map = (const char*(*)(const char*))func;
	}
	else
	{
		mgtk_print("mgtk_link_import> No binding for %s:%p\n", symbol, func);
		return 0;
	}

	return 1;
}


void mgtk_callback_get_image_data_rgb24(const char *filename, 
										unsigned char **image, 
										int *width, int *height)
{
	if (mgtk_callback_get_image_data_rgb24 != NULL)
	{
		(*mgtk_link_callback_get_image_data_rgb24)(filename, image, width, height);
	}
}


void mgtk_handle_application_window_close()
{
	if (mgtk_link_handle_application_window_close != NULL)
	{
		(*mgtk_link_handle_application_window_close)();
	}
}


void mgtk_handle_color(int id, float r, float g, float b, float a)
{
	if (mgtk_link_handle_color != NULL)
	{
		(*mgtk_link_handle_color)(id, r, g, b, a);
	}
}


const char *mgtk_get_resource_path()
{
	if (mgtk_link_get_resource_path != NULL)
	{
		return (*mgtk_link_get_resource_path)();
	}

	return NULL;
}


void mgtk_handle_command( int command )
{
	if ( !ResourceEvent::listen( command - ResourceEvent::eBaseEvent ) )
	{
		mgtk_print( "!%s(%d): Unhandled event.", __func__, command );
	}

	/* Legacy callback. */
	if (mgtk_link_handle_command != NULL)
	{
		(*mgtk_link_handle_command)(command);
	}
}


void mgtk_handle_event2u( int event, unsigned int value, unsigned int value2 )
{
	if ( !ResourceEvent::listen( event - ResourceEvent::eBaseEvent, value, value2 ) )
	{
		mgtk_print( "!%s(%i, %u, %u): Unhandled event.", __func__, event, value, value2 );
	}

	if ( mgtk_link_handle_event2u != NULL )
	{
		(*mgtk_link_handle_event2u)( event, value, value2 );
	}
}


void mgtk_handle_event1u(int event, unsigned int value)
{
	if ( !ResourceEvent::listen( event - ResourceEvent::eBaseEvent, value ) )
	{
		mgtk_print( "!%s(%i, %i): Unhandled event.", __func__, event, value );
	}

	/* Legacy callback. */
	if (mgtk_link_handle_event1u != NULL)
	{
		(*mgtk_link_handle_event1u)(event, value);
	}
}


void mgtk_handle_event1f(int event, float value)
{
	if ( !ResourceEvent::listen( event - ResourceEvent::eBaseEvent, value ) )
	{
		//if ( freyja_event2i(eEvent, event) == -1 )
			mgtk_print( "!%s(%d, %f): Unhandled event.", __func__, event, value );
			//mgtk_print("   mgtk_handle_event1f spawned previous unhandled event %i:%i", eEvent, event);
	}

	/* Legacy callback. */
	if (mgtk_link_handle_event1f != NULL)
	{
		(*mgtk_link_handle_event1f)(event, value);
	}
}


void mgtk_handle_file_dialog_selection(int event, char *filename)
{
	if (mgtk_link_handle_file_dialog_selection != NULL)
	{
		(*mgtk_link_handle_file_dialog_selection)(event, filename);
	}
}


void mgtk_handle_gldisplay()
{
	if (mgtk_link_handle_gldisplay != NULL)
	{
		(*mgtk_link_handle_gldisplay)();
	}
}


void mgtk_handle_glresize(unsigned int width, unsigned int height)
{
	if (mgtk_link_handle_glresize != NULL)
	{
		(*mgtk_link_handle_glresize)(width, height);
	}
}


void mgtk_handle_key_press(int key, int mod)
{
	if (mgtk_link_handle_key_press != NULL)
	{
		(*mgtk_link_handle_key_press)(key, mod);
	}
}


void mgtk_handle_motion( mgtk_mouse_event_t* event )
{
	if ( mgtk_link_handle_motion != NULL )
	{
		(*mgtk_link_handle_motion)( event );
	}
}


void mgtk_handle_mouse(int button, int state, int mod, int x, int y)
{
	if (mgtk_link_handle_mouse != NULL)
	{
		(*mgtk_link_handle_mouse)(button, state, mod, x, y);
	}
}


void mgtk_handle_resource_start()
{
	if (mgtk_link_handle_resource_start != NULL)
	{
		(*mgtk_link_handle_resource_start)();
	}
}


void mgtk_handle_text_array(int event, unsigned int count, char **text_array)
{
	if (mgtk_link_handle_text_array != NULL)
	{
		(*mgtk_link_handle_text_array)(event, count, text_array);
	}
}


void mgtk_handle_text( int event, char* text )
{
	if ( text == NULL ) // || !text[0] )
		return;

	if ( !ResourceEvent::listen( event - ResourceEvent::eBaseEvent, text ) )
	{
		mgtk_print( "%s(%i, '%s'): Unhandled event.", __func__, event, text );
	}

	/* Legacy callback. */
	if (mgtk_link_handle_text != NULL)
	{
		(*mgtk_link_handle_text)(event, text);
	}
}


void mgtk_print( char* format, ... )
{
	va_list args;
	va_start(args, format);

	char buffer[1024];
	vsnprintf(buffer, 1024, format, args);
	va_end(args);

	if (mgtk_link_print != NULL)
	{
		(*mgtk_link_print)(buffer);
	}
}


void mgtk_get_pixmap_filename(char *dest, unsigned int size, char *icon_name)
{
	if (mgtk_link_get_pixmap_filename != NULL)
	{
		(*mgtk_link_get_pixmap_filename)(dest, size, icon_name);
	}
}


const char* mgtk_rc_map(const char* filename_or_dirname)
{
	if (mgtk_link_rc_map != NULL)
	{
		return (*mgtk_link_rc_map)(filename_or_dirname);
	}

	return NULL;
}


