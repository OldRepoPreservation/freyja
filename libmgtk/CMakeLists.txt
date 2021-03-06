# Create the libmgtk library.

# Enable pkgconfig
include(UsePkgConfig)
include(FindOpenGL)


set(Name mgtk)
set(Major 0)
set(Minor 1)
set(Micro 1)
set(Revision 1)
set(Email mongooseichiban@gmail.com)


#enable_testing()

#  GTK_INCLUDE_DIR   - Directories to include to use GTK
#  GTK_LIBRARIES     - Files to link against to use GTK
#  GTK_FOUND         - GTK was found
#  GTK_GL_FOUND      - GTK's GL features were found

add_definitions(
	-DVERSION=\"\\\"lib${Name}-${Major}.${Minor}.${Micro}.${Revision}\\\"\" 
	-DMAJOR_VERSION=${Major}
	-DMINOR_VERSION=${Minor}
	-DMICRO_VERSION=${Micro}
	-DREVISION=${Revision}
	-DBUILD_ID=20070415
	-DEMAIL_ADDRESS=\"\\\"$(Email)\\\"\" 
	-DBUILD_NAME=\"\\\"$(NAME)-$(VERSION)-$(BUILD_ID)\\\"\"

	-DUSING_GTK_2_0 -DHAVE_FREYJA_IMAGE -DHAVE_OPENGL
	-DHAVE_GTKGLEXT -DFORCE_ICON_TOOLBAR -DMGTKEXPORTSCLASS 
)

add_definitions(
	-DEXPERIMENTAL
)

# DEBUG
add_definitions( -DDEBUG -Wall -g )

# RELEASE
#add_definitions( -DRELEASE -DMGTK_ASSERTS -Wall -O2 -finline-functions -fomit-frame-pointer -funroll-loops )

# Lua
FIND_LIBRARY( LUA_FOUND NAMES lua5.1 lua-5.1 )
FIND_PATH( LUA_INCLUDE_PATH lua.h  PATH_SUFFIXES lua5.1 )
FIND_FILE( LUA_INCLUDE lua.h PATH_SUFFIXES lua5.1 )
FIND_FILE( LUALIB_INCLUDE lualib.h PATH_SUFFIXES lua5.1 )
FIND_FILE( LAUXLIB_INCLUDE lauxlib.h PATH_SUFFIXES lua5.1 )
if (LUA_FOUND)

	add_definitions( -DLUA_FOUND -DLUA_FOUND_INCLUDE )
	include_directories( ${LUA_INCLUDE_PATH} )
	link_libraries( ${LUA_FOUND} )
	message( STATUS "Lua 5.1 library found.\n\t${LUA_FOUND}\n\t${LUA_INCLUDE_PATH}\n\t${LUA_INCLUDE}\n\t${LUALIB_INCLUDE}\n\t${LAUXLIB_INCLUDE}" )

elseif ( NOT LUA_FOUND )

	message(FATAL_ERROR "The library LUA 5.1 was not found.  Ensure you have the development libraries installed, and the files are in the correct search path.")

endif (LUA_FOUND)

# LuaWrapper 
add_definitions( -DLUAWRAPPER_FOUND )
include_directories( ../lua )
link_directories( ../lua )
link_libraries(	-lluawrapper )

# libhel
add_definitions( -DHEL_FOUND -DHAVE_LIBHEL -DHAVE_HEL )
include_directories( ../libhel )
link_directories( ../libhel )
link_libraries(	-lhel-0.2.2 )

# OpenGL
include(FindOpenGL)
if (NOT OPENGL_FOUND)
	message(FATAL_ERROR "OpenGL SDK was not found.  Ensure you have the OpenGL SDK installed correctly.")
endif (NOT OPENGL_FOUND)
message(STATUS "OpenGL: Found SDK.")
message(STATUS "	${OPENGL_LIBRARIES}")
add_definitions( -DOPENGL_FOUND -DHAVE_OPENGL )
#include_directories( $(OPENGL_INCLUDE_DIR) )
link_libraries( ${OPENGL_LIBRARIES} )

# Find Gtk
pkgconfig(gtk+-2.0 GTK_INCLUDE_DIR GTK_LIB_DIR GTK_LINK_FLAGS GTK_CFLAGS )
if (NOT GTK_INCLUDE_DIR )
	message(FATAL_ERROR "GTK was not found.  Ensure you have the GTK development libraries installed, and GTK's pkgconfig files are in your pkgconfig search path.")
endif (NOT GTK_INCLUDE_DIR)
#message(STATUS "Gtk+ cflags: ${GTK_CFLAGS}")
message(STATUS "Gtk+: Found SDK.")
message(STATUS "	${GTK_LINK_FLAGS}")
add_definitions( ${GTK_CFLAGS} )
link_directories( ${GTK_LIB_DIR} )
link_libraries( ${GTK_LINK_FLAGS} )
include_directories( ${GTK_INCLUDE_DIR} )

# Find GtkGlExt
pkgconfig(gtkglext-1.0 GTKGLEXT_INCLUDE_DIR GTKGLEXT_LIB_DIR GTKGLEXT_LINK_FLAGS GTKGLEXT_CFLAGS)
if (NOT GTKGLEXT_INCLUDE_DIR)
	message(FATAL_ERROR "GTKGLEXT was not found.  Ensure you have the GTKGLEXT development libraries installed, and GTKGLEXT's pkgconfig files are in your pkgconfig search path.")
endif (NOT GTKGLEXT_INCLUDE_DIR)
message(STATUS "GtkGlExt cflags: ${GTKGLEXT_CFLAGS}")
#add_definitions( ${GTKGLEXT_CFLAGS} -DHAVE_GTKGLAREA )
add_definitions( ${GTKGLEXT_CFLAGS} -DHAVE_GTKGLEXT )
link_directories( ${GTKGLEXT_LIB_DIR} )
link_libraries( ${GTKGLEXT_LINK_FLAGS} )
include_directories( ${GTKGLEXT_INCLUDE_DIR} )

#SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,--enable-runtime-pseudo-reloc -Wl,--export-all-symbols" CACHE STRING "" FORCE)
#gtkglext-1.0

include_directories( ../mstl .  )

set(Objs
	mgtk/ConfirmationDialog
	mgtk/MLisp
	mgtk/QueryDialog
	mgtk/Resource
	mgtk/ResourceEvent
	mgtk/mgtk_callbacks
	mgtk/mgtk_filedialog
	mgtk/mgtk_interface
	mgtk/mgtk_linker
	mgtk/mgtk_lua
	mgtk/mgtk_resource
	mgtk/mgtk_opengl_canvas
	mgtk/mgtk_rangeslider
	mgtk/mgtk_timeslider
	mgtk/mgtk_tree
)


add_library( ${Name}-${Major}.${Minor}.${Micro} SHARED ${Objs} )


#######################################################################
# Packaging
#######################################################################

add_custom_target( deb
	COMMAND fakeroot dpkg-buildpackage )

#add_custom_command(TARGET libhel.deb COMMAND dpkg-buildpackage -rfakeroot)

# Generate a new debian/changelog.
FILE( WRITE debian/changelog "libmgtk (${Major}.${Minor}.${Micro}.${Revision}) feisty; urgency=low\n\n" )
FILE( APPEND debian/changelog "  * Generated release\n\n" )
FILE( APPEND debian/changelog " -- Terry Hendrix (Mongoose) <mongooseichiban@gmail.com>  Thu, 14 Dec 2006 15:52:34 -0500\n\n" )
FILE( APPEND debian/changelog "" )


INSTALL( TARGETS ${Name}-${Major}.${Minor}.${Micro} LIBRARY DESTINATION /usr/lib )


#######################################################################
# Unit tests
#######################################################################

# Remember to run `export LD_LIBRARY_PATH=.` before running CTest!

# mgtk
add_executable( mgtk mgtk/mgtk )
add_dependencies( mgtk ${Name}-${Major}.${Minor}.${Micro} )
set_target_properties( mgtk PROPERTIES OUTPUT_NAME unit/mgtk )
set_target_properties( mgtk PROPERTIES COMPILE_FLAGS 
	"-DUNIT_TEST_MGTK" )
set_target_properties( mgtk PROPERTIES LINK_FLAGS 
	"-L. -l${Name}-${Major}.${Minor}.${Micro} -lpthread" )
add_test( mgtk unit/mgtk unit/mgtk-results.xml )



add_custom_target( win32 
	COMMAND make -f Makefile.Win32 )


