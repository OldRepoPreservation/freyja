/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : pakreader plugin
 * License : No use without permission (c) 2000-2005 Mongoose
 * Comments: This is the backend of the modeler
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.07.23:
 * Mongoose - Created, Based on freyja prototype
 ==========================================================================*/

#include <string.h>
#include <math.h>
#include <hel/math.h>
#include <freyja/FreyjaFileReader.h>
#include <freyja/PerlinNoise.h>
#include <freyja/FreyjaImage.h>
#include <freyja/FreyjaPluginABI.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/mgtk_events.h>


extern "C" {

	void freyja_pakreader_init(void (*func)(const char*, void*));
}

void PakReaderEventsAttach();
void PakReaderGUIAttach();

void freyja_pakreader_init(void (*func)(const char*, void*))
{
	ResourceAppPluginTest *plugin;
	plugin = new ResourceAppPluginTest(PakReaderEventsAttach,
										PakReaderGUIAttach);	
}


void eDialogPakReader()
{
	mgtk_event_dialog_visible_set(Resource::mInstance->getIntByName("eDialogPakReader"), 1);
} 


void PakReaderEventsAttach()
{
	ResourceEventCallback::add("eDialogPakReader", &eDialogPakReader);
}


void PakReaderGUIAttach()
{
	char *filename;
	char *basename = "plugins/pakreader.mlisp";
	int id, menuId;

	id = Resource::mInstance->getIntByName("eDialogPakReader");
	menuId = Resource::mInstance->getIntByName("ePluginMenu");
	mgtk_append_item_to_menu(menuId, "PakReader", id);

	filename = mgtk_rc_map(basename);
	Resource::mInstance->Load(filename);
	delete [] filename;
}






