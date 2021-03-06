/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the Freyja plugin ABI/API.
 * 
 *           
 *-- History ------------------------------------------------ 
 *
 * 2004.12.17: (v0.0.9)
 * Mongoose -  Created, FreyjaPlugin ABI interface defination refinement
 *             based on FreyjaPlugin and FreyjaFSM.
 *
 *             This common interface makes it possible to share ABI 
 *             compatiblity for binary C/C++ plugins.
 *             ( Chimera and Umbra branches merged )
 *
 ==========================================================================*/

#define USING_FREYJA_CPP_ABI

#include <math.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>
#include <zlib.h>
#include "MaterialABI.h"
#include "MeshABI.h"
#include "Metadata.h"
#include "BoneABI.h"
#include "SkeletonABI.h"
#include "PluginABI.h"
#include "PythonABI.h"
#include "Skeleton.h"
#include "XMLSerializer.h"
#include "Plugin.h"

mstl::Vector<freyja::PluginDesc *> gFreyjaPlugins;
mstl::Vector<mstl::String> gPluginDirectories;
mstl::Vector<mstl::String> gImagePluginDirectories;
int32 gCurrentFreyjaPlugin = -1;


using namespace mstl;


//////////////////////////////////////////////////////////////////////
// Plugin C++ ABI
//////////////////////////////////////////////////////////////////////

mstl::Vector<freyja::PluginDesc *> &freyjaGetPluginDescriptions()
{
	return gFreyjaPlugins;
}


mstl::Vector<mstl::String> &freyjaGetPluginDirectories()
{
	return gPluginDirectories;
}


freyja::PluginDesc *freyjaGetPluginClassByFilename(const char *name)
{
	long i, l;

	if (!name || !name[0])
		return 0x0;

	l = strlen(name);

	for (i = gFreyjaPlugins.begin(); i < (long)gFreyjaPlugins.end(); ++i)
	{
		if (gFreyjaPlugins[i] && 
			gFreyjaPlugins[i]->mFilename.c_str() && 
			gFreyjaPlugins[i]->mFilename.c_str()[0])
		{
			if (!strncmp(gFreyjaPlugins[i]->mFilename.c_str(), name, l))
			{
				return gFreyjaPlugins[i];
			}
		}
	}

	return 0x0;
}


freyja::PluginDesc *freyjaGetPluginClassByIndex(long pluginIndex)
{
	if (pluginIndex > 0 && pluginIndex < (long)gFreyjaPlugins.end())
	{
		return gFreyjaPlugins[pluginIndex];
	}

	return 0x0;
}


//////////////////////////////////////////////////////////////////////
// Plugin 0.9.5 ABI
//////////////////////////////////////////////////////////////////////

void freyjaPluginDirectoriesInit()
{
#ifdef WIN32
	freyjaPluginAddDirectory("plugins/model");
	gImagePluginDirectories.push_back(mstl::String("plugins/image"));
#else
	mstl::String s = getenv("HOME");
	s += "/.freyja/plugins/model";
   	freyjaPluginAddDirectory(s.c_str());

	s = getenv("HOME");
	s += "/.freyja/plugins/image";
	gImagePluginDirectories.push_back(s);

	//freyjaPluginAddDirectory("/usr/lib/freyja_0.9.5/modules/model");
	//freyjaPluginAddDirectory("/usr/local/lib/freyja_0.9.5/modules/model");
	//freyjaPluginAddDirectory("/usr/share/freyja/plugins/model");
#endif
}


void freyjaPluginAddDirectory(const char *dir)
{
	if (!dir || !dir[0] || !mstl::SystemIO::File::IsDirectory(dir))
		return;

	uint32 i;
	foreach(gPluginDirectories, i)
	{
		if (!strcmp(gPluginDirectories[i].c_str(), dir))
			return;
	}

	gPluginDirectories.push_back(dir);
}


void freyjaPluginFilename1s(const char *filename)
{

	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		plugin->SetFilename(filename);
}


void freyjaPluginsInit()
{
#ifdef FREYJA_PLUGINS
	mstl::SystemIO::FileReader reader;
	mstl::String module;
	freyja::PluginDesc plugin;
	const char *module_filename;
	void (*init)();
	void *handle;
	unsigned int i;


	gFreyjaPlugins.erase();

	freyjaPluginBegin();
	freyjaPluginDescription1s("All files (*.*)");
	freyjaPluginAddExtention1s("*.*");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | 
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH |
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginEnd();


	freyjaPluginBegin();
	freyjaPluginDescription1s("Freyja Model (*.ja)");
	freyjaPluginAddExtention1s("*.ja");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | 
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_BLENDING |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE); // 2007.06.15 Disabled
	freyjaPluginEnd();


	freyjaPluginBegin();
	freyjaPluginDescription1s("Freyja Model (*.freyja)");
	freyjaPluginAddExtention1s("*.freyja");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | 
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_BLENDING |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH |
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_BLENDING |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginEnd();


	/* Check for other format */
	for (i = gPluginDirectories.begin(); i < gPluginDirectories.end(); ++i)
	{
		if (!reader.OpenDir(gPluginDirectories[i].c_str()))
		{
			freyjaPrintError("Couldn't access plugin directory[%d].", i);
			continue;
		}

		while ((module_filename = reader.GetNextDirectoryListing()))
		{
			if (reader.IsDirectory(module_filename))
				continue;

			if (!mstl::SystemIO::CheckModuleExt(module_filename))
			{
				if (mstl::SystemIO::CheckFilenameExt(module_filename, ".py"))
					freyjaPython1s(module_filename, "<symbol>InitPlugin</symbol>", "init;");
					
				continue;
			}

			DEBUG_MSG("Module '%s' invoked.", module_filename);
			handle = freyjaModuleLoad(module_filename);

			if (handle)
			{
				init = (void (*)())freyjaModuleImportFunction(handle, "freyja_init");

				if (!init)
				{
					freyjaModuleUnload(handle);
					continue;
				}

				freyjaPluginBegin();
				freyjaPluginFilename1s(module_filename);
				(*init)();
				freyjaPluginEnd();

				freyjaModuleUnload(handle);
			}
		}

		reader.Close();
	}

	gCurrentFreyjaPlugin = -1;
#endif
}


int32 freyjaImportModel(const char *filename)
{
	mstl::SystemIO::FileReader reader;

	if (!reader.DoesFileExist(filename))
	{
		freyjaPrintError("File '%s' couldn't be accessed.", filename);
		return -1;
	}

#warning FIXME Add support for native XML format here.


#ifdef FREYJA_PLUGINS
	bool loaded = false, done = false;
	const char *module_filename;
	int (*import)(char *filename);
	void *handle;
	unsigned int i;


	freyjaPrintMessage("[FreyjaPlugin module loader invoked]");

	/* Check for other format */
	for (i = gPluginDirectories.begin(); i < gPluginDirectories.end(); ++i)
	{
		if (!reader.OpenDir(gPluginDirectories[i].c_str()))
		{
			freyjaPrintError("Couldn't access plugin directory[%d].", i);
			continue;
		}

		while (!done && (module_filename = reader.GetNextDirectoryListing()))
		{
			if (reader.IsDirectory(module_filename))
				continue;

			if (!mstl::SystemIO::CheckModuleExt(module_filename))
			{
				if (mstl::SystemIO::CheckFilenameExt(module_filename, ".py"))
					freyjaPython1s(module_filename, "<symbol>ImportModel</symbol>", filename);
					
				continue;
			}

			freyjaPrintMessage("Module '%s' invoked.", module_filename);

			if (!(handle = freyjaModuleLoad(module_filename)))
			{
				continue; /* Try the next plugin, after a bad module load */
			}
			else
			{
				freyjaPrintMessage("Module '%s' opened.", module_filename);

#ifdef NO_DUPE_DL_SYM_HACK
				/* Mongoose 2004.11.01, 
				 * temp fix? */
				unsigned int l = strlen(module_filename);
				char tmp[128];
				module_filename[l-3] = 0;
				snprintf(tmp, 64, "%s_import_model", basename(module_filename));
				freyjaPrintMessage("Symbol '%s' import...", tmp);
				import = (int (*)(char *filename))freyjaModuleImportFunction(handle, tmp);
#else
				import = (int (*)(char *filename))freyjaModuleImportFunction(handle, "import_model");
#endif

				if (!import)  
				{
					freyjaModuleUnload(handle);
					continue;
				}


				freyja::PluginDesc *plug = freyjaGetPluginClassByFilename(module_filename); 

				if (plug)
					gCurrentFreyjaPlugin = plug->GetId(); 

				done = !(*import)((char*)filename);

				gCurrentFreyjaPlugin = -1;

				if (done)
				{
					loaded = true;
					freyjaPrintMessage("Module '%s' success.", module_filename);
				}

				freyjaModuleUnload(handle);
			}
		}

		reader.CloseDir();

		if (done)
		{
			break;
		}
	}

	freyjaPrintMessage("[FreyjaPlugin module loader sleeps now]\n");

	if (loaded)
		return 0; // success
#endif

	return -1;
}


int gzwrite_buffer(const char* filename, const char* buffer, const unsigned int size)
{
#if 0
	const unsigned int chunk = 16384;
	byte out[chunk];

    /* Allocate deflate state */
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

	/* Use deflateInit2 / inflateInit2 functions for Gzip format. */
    int ret = deflateInit2(&strm, level);

    if (ret != Z_OK)
        return ret;

    /* Compress xml string. */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);	

    /* Clean up. */
    (void)deflateEnd(&strm);

	return ( ret != Z_STREAM_END ) ? -1 : 0;
#else
	 gzFile f = gzopen(filename, "wb");

	 if ( f )
	 {
		 if ( gzwrite(f, buffer, size) < (int)size )
		 {
			 freyjaPrintError("gzwrite('%s') failed.", filename);
		 }
			 
		gzclose(f);
		return 0;
	 }

	 return -1;
#endif
}


int gzread_buffer(const char* filename, char*& buffer, const unsigned int& size)
{
	 gzFile f = gzopen(filename, "rb");

	 if ( f )
	 {
		 if ( gzread(f, buffer, size) < (int)size )
		 {
			 freyjaPrintError("gzread('%s') failed.", filename);
		 }

		 gzclose(f);
		 return 0;
	 }

	 return -1;
}


int freyjaExportScene(const char* filename)
{
#if TINYXML_FOUND
	if ( !filename || !filename[0] )
		return -1;

	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild( decl );

	TiXmlElement* root = new TiXmlElement("freyja");
	root->SetAttribute("version", VERSION);
	root->SetAttribute("build-date", __DATE__);
	root->SetAttribute("file-version", 1);
	doc.LinkEndChild( root );

	// Materials
	TiXmlElement* materials = new TiXmlElement("materials");
	root->LinkEndChild( materials );
	//for (uint32 i = 0, n = freyjaGetMaterialCount(); i < n; ++i)
	//{
		//FIXME freyja::Material *mat = freyjaGetMaterialClass( i );
		//
		//if (mat)
		//{
		//	mat->Serialize( materials );
		//}
	//}

	// Scene
	TiXmlElement* scene = new TiXmlElement("scene");
	root->LinkEndChild( scene );

	//    Metadata
	//for (uint32 i = 0, n = Metadata::GetObjectCount(); i < n; ++i)
	//{
		//FIXME
		//freyja::Metadata* metadata = Metadata::GetObjectByUid( i );
		//
		//if (metadata)
		//{
		//	metadata->Serialize( materials );
		//}
	//}
	


	//       Skeletons
	//          Bones
	//

	//    Models
	TiXmlElement* models = new TiXmlElement("models");
	scene->LinkEndChild( models );


	// foreach()
	TiXmlElement* model = new TiXmlElement("model");
	models->LinkEndChild( model );

	//       SkeletonInstance

	//       Meshes
#if 0
	for (uint32 i = 0, n = Mesh::GetCount(); i < n; ++i)
	{
		Mesh* mesh = Mesh::GetMesh( i );

		if (mesh)
		{
			mesh->Serialize( model );
		}
	}
#endif

	TiXmlPrinter printer;
	printer.SetIndent( "\t" );
	doc.Accept( &printer );
	const char* xml = printer.CStr();

	if ( xml )
	{
		gzwrite_buffer( filename, xml, strlen(xml) );
	}

	//doc.SaveFile( filename );

	return 0;

#else

	return -1;

#endif // TINYXML_FOUND
}


int freyjaImportScene(const char* filename)
{
#if TINYXML_FOUND

	if ( !filename || !filename[0] )
		return -1;

	TiXmlDocument doc;
	char* xml;
	unsigned int size;

	gzread_buffer( filename, xml, size );
	doc.Parse( xml );

	delete xml; // this smart?
                        
	if ( doc.Error() )
	{
		return -2;
	}

	return 0;

#else

	return -1;

#endif // TINYXML_FOUND
}


int32 freyjaExportModel(const char *filename, const char *type)
{
	if (!type || !filename)
		return -100;

	/* Check for native format. */
	if ( strcmp(type, "freyja") == 0 )
	{
		if ( !freyjaExportScene( filename ) )
		{
			return 0;
		}
	}


#ifdef FREYJA_PLUGINS
	mstl::SystemIO::FileReader reader;
	mstl::String module, symbol;
	bool saved = false;
	int (*export_mdl)(char *filename);
	void *handle;

	freyjaPrintMessage("[FreyjaPlugin module loader invoked]\n");
	char* name = (char*)type;

	/* Check for other format */
	for (unsigned long i = gPluginDirectories.begin(); i < gPluginDirectories.end(); ++i)
	{
		if (!reader.OpenDir(gPluginDirectories[i].c_str()))
		{
			freyjaPrintError("Couldn't access plugin directory");
			continue;
		}

		module.Set("%s/%s%s", 
				   gPluginDirectories[i].c_str(), name, 
				   mstl::SystemIO::GetModuleExt());
		symbol.Set("freyja_model__%s_export", name);


		if (!(handle = freyjaModuleLoad(module.c_str())))
		{
			module.Set("%s/%s.py", gPluginDirectories[i].c_str(), name);
			freyjaPython1s(module.c_str(), "<symbol>ExportModel</symbol>", filename);
		}
		else
		{
			freyjaPrintMessage("\tModule '%s' opened.\n", module.c_str());
    
			export_mdl = (int (*)(char * filename))freyjaModuleImportFunction(handle, symbol.c_str());

			freyja::PluginDesc *plug = freyjaGetPluginClassByFilename(module.c_str()); 

			if (plug)
				gCurrentFreyjaPlugin = plug->GetId(); 

			if (export_mdl)
				saved = (!(*export_mdl)((char*)filename));

			gCurrentFreyjaPlugin = -1;

			freyjaModuleUnload(handle);
		}

		if (saved)
			break;
	}

	freyjaPrintMessage("[FreyjaPlugin module loader sleeps now]\n");

	if (saved)
		return 0; // success
#endif

	return -1;
}


int32 freyjaExportModelByModule(const char *filename, const char *module)
{
#ifdef FREYJA_PLUGINS
	freyja::PluginDesc *plugin = freyjaGetPluginClassByFilename(module);

	if (!plugin || !mstl::SystemIO::File::DoesFileExist(plugin->mFilename.c_str()))
	{
		freyjaPrintError("Module '%s' couldn't be found.", module);
		return -1;
	}
												
	mstl::String symbol = "freyja_model__" + plugin->mName;
	symbol += "_export";
	freyjaPrintError("! *** %s", symbol.c_str());

	bool saved = false;
	int (*export_func)(char *filename);
	void *handle = freyjaModuleLoad(module);

	if (handle)
	{
		freyjaPrintMessage("\tModule '%s' opened.\n", module);

		export_func = (int (*)(char * filename))freyjaModuleImportFunction(handle, symbol.c_str());

		gCurrentFreyjaPlugin = plugin->GetId(); 

		if (export_func)
			saved = (!(*export_func)((char*)filename));

		gCurrentFreyjaPlugin = -1;

		freyjaModuleUnload(handle);
	}

	return saved ? 0 : -2;
#endif

	return -1;
}


int32 freyjaImportModelByModule(const char *filename, const char *module)
{
#ifdef FREYJA_PLUGINS
	freyja::PluginDesc *plugin = freyjaGetPluginClassByFilename(module);

	if (!plugin || !SystemIO::File::DoesFileExist(plugin->mFilename.c_str()))
	{
		freyjaPrintError("Module '%s' couldn't be found.", module);
		return -1;
	}
												
	String symbol = "freyja_model__";
	symbol += plugin->mName;
	symbol += "_import";

	freyjaPrintError("! *** %s", symbol.c_str());

	bool loaded = false;
	int (*import_func)(char *filename);
	void *handle = freyjaModuleLoad(module);

	if (handle)
	{
		freyjaPrintMessage("\tModule '%s' opened.\n", module);

		import_func = (int (*)(char * filename))freyjaModuleImportFunction(handle, symbol.c_str());

		gCurrentFreyjaPlugin = plugin->GetId(); 

		if (import_func)
			loaded = (!(*import_func)((char*)filename));

		gCurrentFreyjaPlugin = -1;

		freyjaModuleUnload(handle);
	}

	return loaded ? 0 : -2;
#endif

	return -1;
}


void freyjaPluginName(uint32 pluginIndex, const char *name)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginIndex);

	if (plugin)
	{
		plugin->SetName(name);
	}
}


void freyjaPluginDescription(uint32 pluginIndex, const char *info_line)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginIndex);

	if (plugin)
	{
		plugin->SetDescription(info_line);
	}
}


void freyjaPluginImportFlags(uint32 pluginIndex, int32 flags)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginIndex);

	if (plugin)
	{
		plugin->mImportFlags = flags;
	}
}


void freyjaPluginExportFlags(uint32 pluginIndex, int32 flags)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginIndex);

	if (plugin)
	{
		plugin->mExportFlags = flags;
	}
}

void freyjaPluginExtention(uint32 pluginIndex, const char *ext)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginIndex);

	if (plugin)
	{
		plugin->SetExtention(ext);
	}
}


uint32 freyjaGetPluginCount()
{
	return gFreyjaPlugins.end();
}


void freyjaPluginShutdown()
{
	gFreyjaPlugins.erase();
}


const char* freyjaPluginFindByDescription(const char* desc)
{
	freyja::PluginDesc *module = NULL;

	for (uint32 i = 0, count = freyjaGetPluginCount(); i < count; ++i)
	{
		freyja::PluginDesc* plugin = freyjaGetPluginClassByIndex( i );
			
		if ( plugin == NULL )
			continue;

		if ( plugin->mDescription == desc )
		{
			module = plugin;
			break;
		}
	}

	return (module) ? module->mFilename.c_str() : NULL;
}



///////////////////////////////////////////////////////////////////////
//  Plugin import/export iteraction
///////////////////////////////////////////////////////////////////////

void freyjaPluginBegin()
{
	freyja::PluginDesc *plugin = new freyja::PluginDesc();
	plugin->SetId(gFreyjaPlugins.size());
	gFreyjaPlugins.push_back(plugin);
	
	gCurrentFreyjaPlugin = plugin->GetId();
}


void freyjaPluginName1s(const char *name)
{
	//OBS_CALL(freyjaPluginName1s);
	freyjaPluginName(gCurrentFreyjaPlugin, name);
}


void freyjaPluginDescription1s(const char *info_line)
{
	//OBS_CALL(freyjaPluginDescription1s);
	freyjaPluginDescription(gCurrentFreyjaPlugin, info_line);
}


void freyjaPluginAddExtention1s(const char *ext)
{
	//OBS_CALL(freyjaPluginAddExtention1s);
	freyjaPluginExtention(gCurrentFreyjaPlugin, ext);
}


void freyjaPluginImport1i(int32 flags)
{
	//OBS_CALL(freyjaPluginImport1i);
	freyjaPluginImportFlags(gCurrentFreyjaPlugin, flags);
}


void freyjaPluginExport1i(int32 flags)
{
	//OBS_CALL(freyjaPluginExport1i);
	freyjaPluginExportFlags(gCurrentFreyjaPlugin, flags);
}

//FIXME these need direct index functions too
void freyjaPluginArg1i(const char *name, int32 defaults)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		plugin->AddIntArg(name, defaults);
}


void freyjaPluginArg1f(const char *name, float defaults)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		plugin->AddFloatArg(name, defaults);
}


void freyjaPluginArg1s(const char *name, const char *defaults)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		plugin->AddStringArg(name, defaults);
}


void freyjaPluginEnd()
{
	// ATM this does nothing, just here for reserved use
}


int32 freyjaGetPluginId()
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		return plugin->GetId();

	return -1;
}


int freyjaGetPluginArg1f(int32 pluginId, const char *name, float *arg)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginId);

	if (plugin)
	{
		*arg =  plugin->GetFloatArg(name);
		return 0;
	}

	return -1;
}


int freyjaGetPluginArg1i(int32 pluginId, const char *name, int32 *arg)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginId);

	if (plugin)
	{
		*arg =  plugin->GetIntArg(name);
		return 0;
	}

	return -1;
}


int freyjaGetPluginArg1s(int32 pluginId, const char *name, char **arg)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginId);

	if (plugin)
	{
		*arg =  plugin->GetStringArg(name);
		return 0;
	}

	return -1;
}


int freyjaGetPluginArgString(int32 pluginId, const char *name, 
							 int32 len, char *arg)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginId);
	char *s = 0x0;

	if (plugin)
		s = plugin->GetStringArg(name);

	if (!s || !s[0])
		return -1;

	strncpy(arg, s, len);

	return 0;
}

