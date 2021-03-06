################################################################
# Mongoose <mongooseichiban@gmail.com>
################################################################
# + Cleaner clean
# + Add new include tree
# + Add new flags
# + Install/Uninstall
# + Lots of misc new features
# + Stripped down for module/plugin build
################################################################

NAME=skel
BASE_NAME=$(NAME)
MODULE=$(NAME)
TREE_DIR=$(NAME)
MAJOR_VERSION=0
MINOR_VERSION=9
MICRO_VERSION=6
BRANCH=pre1
BUILD_ID=20060610
VERSION=$(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)$(BRANCH)
BUILD_HOST=`uname -n -m -s`
WEBSITE="http://icculus.org/freyja"
EMAIL="mongoose@icculus.org"
EXT=.so
TARBALL_NAME=$(NAME)-$(VERSION).$(BUILD_ID)$(BRANCH)
LICENSE=LGPL


################################################################
# Build options
################################################################

BUILD_SELECT=debug
BASE_DEFS=-DMODEL_PLUGINS
DEBUG_DEFS=-DDEBUG -DEXPERIMENTAL
CONFIGURE_DEF=-DFREYJAIMAGE_PLUGINS -DFREYJA_PLUGINS
CONFIGURE_LNK=
BUILD_ROOT_DIR=../../../../bin/plugins/model/$(NAME)
BUILD_DEBUG_DIR=$(BUILD_ROOT_DIR)/debug
BUILD_RELEASE_DIR=$(BUILD_ROOT_DIR)/release
BUILD_PROF_DIR=$(BUILD_ROOT_DIR)/prof
BUILD_WIN32_DIR=$(BUILD_ROOT_DIR)/win32
BUILD_TEST_DIR=$(BUILD_ROOT_DIR)/test
BUILD_INSTALL_DIR=../../../../bin/plugins/model
SRC_DIR=.


################################################################
# Compiler and Cross Environment
################################################################

CC=gcc
DEPS=../../../deps.sh
-include ../../../../win32.env


################################################################
# Compiler flags
################################################################

BASE_CFLAGS=-fPIC -Wall -I$(SRC_DIR) \
	$(CONFIGURE_DEF) \
	-DPLUGIN_IMAGE_DIR=\"\\\"$(INSTALL_PLUGIN_DIR)/image\\\"\" \
	-DVERSION=\"\\\"$(VERSION)-$(BUILD_ID)\\\"\" \
	-DPROJECT_URL=\"\\\"$(WEBSITE)\\\"\" \
	-DEMAIL_ADDRESS=\"\\\"$(EMAIL)\\\"\" \
	$(BASE_DEFS)

LDFLAGS=$(CONFIGURE_LNK) -lm -ldl
#LDFLAGS += -Wl,-z,defs
#LDFLAGS += -Wl,-O1
RELEASE_CFLAGS=$(BASE_CFLAGS) -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations -O2
DEBUG_CFLAGS=$(BASE_CFLAGS) -g $(DEBUG_DEFS)

################################################################
# Local Build Environment
################################################################

-include ../../../freyja.env


# Library ######################################################
SHLIBEXT=$(EXT).$(VERSION)
SHLIBLDFLAGS=$(LDFLAGS) -fPIC -shared

# Module #######################################################
MODULE_CFLAGS=$(BASE_CFLAGS) -g -DFREYJA_PLUGINS
MODULE_LD_FLAGS=-lfreyja -lstdc++ -shared -fPIC -ldl -rdynamic 
#MODULE_LD_FLAGS += --warn-unresolved-symbols -Wl -z defs
MODULE_LD_FLAGS += --warn-unresolved-symbols -Wl

# Unit test ####################################################
SDLUNIT_CFLAGS=$(BASE_CFLAGS) $(shell sdl-config --cflags) \
		-DHAVE_SDL -DHAVE_OPENGL -DHAVE_SDL_TTF -DHAVE_FREYJA
SDLUNIT_LDFLAGS=$(shell sdl-config --libs) -lSDL_ttf -lGL -lGLU \
		-lm -lstdc++ -lhel0 -lfreyja
SDLUNIT_OBJS=$(BUILD_TEST_DIR)/SDLUnitTest.o


################################################################
# Build rules
################################################################

DO_CC=$(CC) $(CFLAGS) -o $@ -c $<
DO_SHLIB_CC=$(CC) $(CFLAGS) $(SHLIBCFLAGS) -o $@ -c $<
TARGETS=$(BUILDDIR)/$(NAME)$(SHLIBEXT)

auto: $(BUILD_SELECT)

all: debug release prof

debug:
	@-mkdir -p $(BUILD_DEBUG_DIR)
	$(MAKE) targets BUILDDIR=$(BUILD_DEBUG_DIR) \
	SHLIBEXT=".so" \
	CFLAGS="$(DEBUG_CFLAGS)" \
	LD_FLAGS="$(LDFLAGS)"
	@-cp $(BUILD_DEBUG_DIR)/$(NAME).so $(BUILD_INSTALL_DIR)/debug

prof:
	@-mkdir -p $(BUILD_PROF_DIR)
	$(MAKE) targets BUILDDIR=$(BUILD_PROF_DIR) \
	SHLIBEXT=".so" \
	CFLAGS="$(DEBUG_CFLAGS) -pg" \
	LD_FLAGS="$(LDFLAGS) -pg"

release:
	@-mkdir -p $(BUILD_RELEASE_DIR)
	$(MAKE) targets BUILDDIR=$(BUILD_RELEASE_DIR) \
	SHLIBEXT=".so" \
	CFLAGS="$(RELEASE_CFLAGS)" \
	LD_FLAGS="$(LDFLAGS)"

win32:
	@-mkdir -p $(BUILD_WIN32_DIR)
	$(MAKE) targets BUILDDIR=$(BUILD_WIN32_DIR) \
	SHLIBEXT=".dll" \
	CC=$(WIN32_CC) \
	CPP=$(WIN32_CPP)  \
	LD=$(WIN32_LD)  \
	CFLAGS="$(BASE_CFLAGS) $(WIN32_INC_DIR)" \
	LD_FLAGS="$(CONFIGURE_LNK) -lm" \
	LIBS="-lstdc++ -lm $(WIN32_LIB_DIR) $(WIN32_LIBHEL) $(WIN32_LIBFREYJA)"

osx:
	@-mkdir -p $(BUILD_DEBUG_DIR)
	$(MAKE) targets BUILDDIR=$(BUILD_DEBUG_DIR) \
	SHLIBEXT=".bundle" \
	CFLAGS="$(DEBUG_CFLAGS) $(STD_PLUGIN_INC)" \
	LD_FLAGS="$(CONFIGURE_LNK) -lm -ldl $(STD_PLUGIN_LIBS)"


################################################################
# Include Dependences
################################################################

depend:
	@-echo -n Making deps...
	@-echo "# Autogenerated dependency file" > depend
	@-find $(SRC_DIR) -name "*.cpp" -exec $(DEPS) $(BASE_DEFS) {} \; >> depend
	@-echo "       [done]"


################################################################
# Main targets and objects
################################################################

targets: $(TARGETS)

PROG_OBJS=$(BUILDDIR)/Skel.o

$(BUILDDIR)/$(MODULE) : $(PROG_OBJS)
	$(CC) $(CFLAGS) $(LD_FLAGS) -o $@ $(PROG_OBJS)

$(BUILDDIR)/$(MODULE).test : $(PROG_OBJS)
	$(CC) $(CFLAGS) $(LD_FLAGS) -o $@ $(PROG_OBJS)

$(BUILDDIR)/$(MODULE).so : $(PROG_OBJS)
	$(CC) $(CFLAGS) $(SHLIBLDFLAGS) -o $@ $(PROG_OBJS)

$(BUILDDIR)/$(MODULE).dll : $(PROG_OBJS)
	@-mkdir -p $(WIN32_MODEL_PLUGIN_DIR)
	$(CC) -shared -o $(WIN32_MODEL_PLUGIN_DIR)/$(MODULE).dll \
	-Wl,--out-implib=$(BUILDDIR)/lib$(MODULE).a \
	-Wl,--export-all-symbols \
	-Wl,--enable-auto-import \
	-Wl,--whole-archive $(PROG_OBJS) \
	-Wl,--no-whole-archive $(LIBS)

$(BUILDDIR)/$(MODULE).bundle : $(PROG_OBJS)
	$(CC) $(LD_FLAGS) \
	-L/usr/lib -lstdc++ -bundle -flat_namespace -undefined suppress \
	-o $(BUILDDIR)/$(MODULE).bundle \
	$(PROG_OBJS)


######################################################################
# Implemented builds
######################################################################

#tools:

test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) targets NAME=ase SHLIBEXT=.test BUILDDIR=$(BUILD_TEST_DIR) \
	PROG_OBJS="$(BUILD_TEST_DIR)/Ase.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_ASE $(BASE_CFLAGS)" \
	LD_FLAGS="-lm -ldl -lstdc++ -lhel0 -lfreyja"


##########################################################################
# Object dependency include
##########################################################################

-include depend


##########################################################################
# Make clean
##########################################################################

clean: clean-emacs clean-build clean-test clean-obj clean-win32

clean-test:
	@-echo -n "Cleaning test builds                         "
	@-rm -f $(BUILD_TEST_DIR)/*.test
	@-echo "[done]"

clean-obj:
	@-echo -n "Cleaning objects                             "
	@-rm -f $(BUILD_PROF_DIR)/*.o
	@-rm -f $(BUILD_DEBUG_DIR)/*.o
	@-rm -f $(BUILD_RELEASE_DIR)/*.o
	@-rm -f $(BUILD_TEST_DIR)/*.o
	@-echo "[done]"

clean-win32:
	@-echo -n "Cleaning WIN32 builds                        "
	@-rm -f $(BUILD_WIN32_DIR)/*.o
	@-rm -f $(BUILD_WIN32_DIR)/*.a
	@-rm -f $(BUILD_WIN32_DIR)/*.dll
	@-echo "[done]"

clean-emacs:
	@-echo -n "Cleaning emacs files                         "
	@-rm -f `find . -name "*~" -print`
	@-echo "[done]"

clean-build:
	@-echo -n "Cleaning builds                              "
	@-rm -f $(BUILD_PROF_DIR)/$(NAME)$(SHLIBEXT)
	@-rm -f $(BUILD_DEBUG_DIR)/$(NAME)$(SHLIBEXT)
	@-rm -f $(BUILD_RELEASE_DIR)/$(NAME)$(SHLIBEXT)
	@-rm -f $(BUILD_PROF_DIR)/*.so
	@-rm -f $(BUILD_DEBUG_DIR)/*.so
	@-rm -f $(BUILD_RELEASE_DIR)/*.so
	@-rm -f $(BUILD_DEBUG_DIR)/*.test
	@-echo "[done]"


##########################################################################
# Source tree backup
##########################################################################

tarball:
	@-echo "Making tarball                                  " 
	@-cd .. && tar zcvf $(TARBALL_NAME).tar.gz  \
		--exclude=bin --exclude=.svn --exclude=*~ --exclude=contrib \
		--totals \
		$(TREE_DIR)
	@-echo "[done]"

##########################################################################

