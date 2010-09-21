#/////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
#//
#// Distributed under the Boost Software License, Version 1.0.
#// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#/////////////////////////////////////////////////////////////////////////////////////////////


# A very dirty makefile.. but does the job


PROJECT_ROOT := ..

INSTALL      := false
INSTALL_PATH ?= 

PROGRAM      := test_ocv_matrix
LIBRARY_ST   := 
LIBRARY_SH   := 
PKGNAME      := 
TEST         := test_ocv_matrix
PURGE        := 

DEPLIBS      := cxcore cppunit dl
LDFLAGS      := 

CXX          := g++
CXXFLAGS     := -Wall -Werror -ansi -pedantic -ffloat-store -O3 -std=gnu++0x



ifneq "$(TEST)" ""
 SRCDIR      := .
else
 SRCDIR      := src
endif
INCDIR       := api
BINDIR       := bin
LIBDIR       := lib
INCNAME      := include


ARCH         := $(shell uname -m)
ARCHFLAGS    := -D$(ARCH)
BOOSTFLAGS   := -DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG -DNDEBUG
CXXFLAGS     += $(BOOSTFLAGS) $(ARCHFLAGS)

OBJDIR       := obj
OBJDIR_ST    := obj
OBJDIR_SH    := obj_pic


IINCL        := $(shell echo $(C_INCLUDE_PATH) | sed 's/:*$$//; s/^:*/ -I/; s/::*/ -I/g')
INCLUDES     := -I$(PROJECT_ROOT)/$(INCNAME) $(IINCL)
INCLIST      := $(shell find $(PROJECT_ROOT)/$(INCDIR) -ignore_readdir_race -name "*.h")

LLIBS        := $(shell echo $(LD_LIBRARY_PATH) | sed 's/:*$$//; s/^:*/ -L/; s/::*/ -L/g')
LDFLAGS      += -L$(PROJECT_ROOT)/lib $(LLIBS)
LDLIBS       += $(addprefix -l,$(DEPLIBS))


SRC          := $(wildcard $(SRCDIR)/*.cc)
OBJECTS      := $(patsubst %.cc,$(OBJDIR)/%.o,$(notdir $(SRC)))
OBJECTS_ST   := $(patsubst %.cc,$(OBJDIR_ST)/%.o,$(notdir $(SRC)))
OBJECTS_SH   := $(patsubst %.cc,$(OBJDIR_SH)/%.o,$(notdir $(SRC)))

BINNAME      := $(BINDIR)/$(PROGRAM)
STATIC_LIB   := $(LIBDIR)/lib$(LIBRARY_ST).a
SHARED_LIB   := $(LIBDIR)/lib$(LIBRARY_SH).so


TARGETS      := $(INCNAME)
ifneq "$(PROGRAM)" ""
 TARGETS     += $(BINNAME)
endif
ifneq "$(LIBRARY_ST)" ""
 TARGETS     += $(STATIC_LIB)
endif
ifneq "$(LIBRARY_SH)" ""
 TARGETS     += $(SHARED_LIB)
endif
ifneq "$(TEST)" ""
 TARGETS     += runtest
endif



all: $(TARGETS)


$(STATIC_LIB): $(OBJECTS_ST)
	@echo "  linking $(@F)"
	@mkdir -p $(LIBDIR)
	@ar r $@ $(OBJECTS_ST) 2>/dev/null; ranlib $@


$(SHARED_LIB): $(OBJECTS_SH)
	@echo "  linking $(@F)"
	@mkdir -p $(LIBDIR)
	@$(CXX) -shared $(OBJECTS_SH) -o $@


$(BINNAME): $(OBJECTS)
	@echo "  linking $(@F)"
	@mkdir -p $(BINDIR)
	@$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)


$(OBJDIR_ST)/%.o: $(SRCDIR)/%.cc $(INCLIST)
	@echo "  compiling static $(patsubst $(OBJDIR_ST)/%.o,%.cc,$@)"
	@mkdir -p $(OBJDIR_ST)
	@$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDES)


$(OBJDIR_SH)/%.o: $(SRCDIR)/%.cc $(INCLIST)
	@echo "  compiling shared $(patsubst $(OBJDIR_SH)/%.o,%.cc,$@)"
	@mkdir -p $(OBJDIR_SH)
	@$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@ $(INCLUDES)


$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(INCLIST)
	@echo "  compiling $(patsubst $(OBJDIR)/%.o,%.cc,$@)"
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDES)


$(INCNAME): $(INCLIST)
	@if [ "$(TEST)" = "" ]; then \
	    echo "  pusblishing header files"; \
	    if [ ! -d $(PROJECT_ROOT)/$(INCNAME)/$(PKGNAME) ]; then \
	       install --directory --mode=775 $(PROJECT_ROOT)/$(INCNAME)/$(PKGNAME); \
	    fi; \
	    `cp -rf $(INCDIR)/* $(PROJECT_ROOT)/$(INCNAME)/$(PKGNAME)/`; \
	    `find $(PROJECT_ROOT)/$(INCNAME)/$(PKGNAME)/ -type d -exec chmod 775 {} \;` ; \
	    `find $(PROJECT_ROOT)/$(INCNAME)/$(PKGNAME)/ -name "*.h" -exec chmod 444 {} \;` ; \
	fi;




CLEANTARGETS := $(OBJDIR) $(OBJDIR_ST) $(OBJDIR_SH)
CLOBBTARGETS := $(BINDIR) $(LIBDIR) $(INCNAME) $(OBJDIR) $(OBJDIR_ST) $(OBJDIR_SH) $(PURGE)


install: all
	@if [ "$(INSTALL)" = "true" ]; then \
	    if [ "$(INSTALL_PATH)" = "" ]; then \
	       echo "INSTALL_PATH not set"; \
	       exit 1; \
	    fi; \
	    if [ "$(PROGRAM)" ] && [ "$(TEST)" = "" ]; then \
	       echo "  installing $(PROGRAM)"; \
	       if [ ! -d $(INSTALL_PATH)/bin ]; then \
	          install --directory --mode=775 $(INSTALL_PATH)/bin; \
	       fi; \
	       install --mode=555 --strip $(BINNAME) $(INSTALL_PATH)/bin; \
	    fi; \
	    if [ "$(LIBRARY_ST)" ] || [ "$(LIBRARY_SH)" ]; then \
	       if [ "$(LIBRARY_ST)" ]; then \
	          echo "  installing lib$(LIBRARY_ST).a"; \
	          if [ ! -d $(INSTALL_PATH)/lib ]; then \
	             install --directory --mode=775 $(INSTALL_PATH)/lib; \
	          fi; \
	          install --mode=555 $(STATIC_LIB) $(INSTALL_PATH)/lib; \
	       fi; \
	       if [ "$(LIBRARY_SH)" ]; then \
	          echo "  installing lib$(LIBRARY_SH).a"; \
	          if [ ! -d $(INSTALL_PATH)/lib ]; then \
	             install --directory --mode=775 $(INSTALL_PATH)/lib; \
	          fi; \
	          install --mode=555 --strip $(SHARED_LIB) $(INSTALL_PATH)/lib; \
	       fi; \
	    fi; \
	    if [ "$(PKGNAME)" ]; then \
	       echo "  installing header files"; \
	       if [ ! -d $(INSTALL_PATH)/include/$(PKGNAME) ]; then \
	          install --directory --mode=775 $(INSTALL_PATH)/include/$(PKGNAME); \
	       fi; \
	       `cp -rf $(INCDIR)/* $(INSTALL_PATH)/include/$(PKGNAME)/` ; \
	       `find $(INSTALL_PATH)/include/$(PKGNAME)/ -name "*.h" -exec chmod 444 {} \;` ; \
	       `find $(INSTALL_PATH)/include/$(PKGNAME)/ -type d -exec chmod 775 {} \;` ; \
	    fi; \
	 fi


runtest: $(BINNAME)
	@echo "  testing $(PROGRAM)"
	@export LD_LIBRARY_PATH=$(PROJECT_ROOT)/$(LIBDIR):$(LD_LIBRARY_PATH)
	@export PATH=$(BINDIR):$(PATH)
	@$(BINDIR)/$(TEST)


clean:
	@echo "  cleaning..."
	@rm -rf $(CLEANTARGETS)

clobber: clean
	@echo "  clobbering..."
	@rm -rf $(CLOBBTARGETS)



.PRECIOUS: $(OBJECTS) $(OBJDIR_ST) $(OBJDIR_SH)

.PHONY: all install clean clobber runtest

