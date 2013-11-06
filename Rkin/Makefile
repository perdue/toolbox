# Name of shared library
LIBNAME = Rkin

# Path to libg2c
#G2CPATH = /usr/lib
G2CPATH = /usr/lib64

# All temporary files are placed here,
# such as cint dictionaries and object files.
TMPDIR = ../tmp

# All libraries are placed here.
LIBDIR = ../lib

# If a file that is listed as a prerequisite does not exist in the current directory,
# 'make' searches the directories listed in VPATH for a file with that name.
VPATH = $(TMPDIR)

MYINCLUDES = -I../$(LIBNAME)

CXXFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags) -fPIC -Wall
#CXXFLAGS += -m32
CXXFLAGS += $(MYINCLUDES)
LDFLAGS += $(shell $(ROOTSYS)/bin/root-config --glibs) -Wall
#LDFLAGS += -m32

HDRS = $(LIBNAME).h
OBJS = $(TMPDIR)/rkin.o $(TMPDIR)/$(LIBNAME).o $(TMPDIR)/$(LIBNAME)Dictionary.o

F77 = g77

howto_Rkin: $(TMPDIR)/howto_Rkin.o $(LIBDIR)/lib$(LIBNAME).so
	@echo "   Compiling: $^ ---> $@"
	@$(CXX) $(LDFLAGS) $^ -o $@ -L$(LIBDIR) -lRkin

# Creating shared object library.
$(LIBDIR)/lib$(LIBNAME).so: $(OBJS)
	@echo "   Creating Shared Lib: $^ ---> $@"
	@$(CXX) $(LDFLAGS) -shared $^ $(G2CPATH)/libg2c.so.0 -o $@

# Compiling all C++ source files, including dictionary source file, using a pattern rule.
$(TMPDIR)/%.o : %.cxx
	@echo "   Compiling: $^ ---> $@"
	@$(CXX) -c $(CXXFLAGS) $^ -o $@

# Update time stamp of source file if header is edited.
%.cxx: %.h
	@touch $@

# Creating dictionary code from header files using rootcint.
$(TMPDIR)/$(LIBNAME)Dictionary.cxx: $(HDRS)
	@echo "   Making Dictionary: $^ ---> $@"
	@rootcint -f $@ -c $(MYINCLUDES) $^


# Compiling all fortran source files using a pattern rule.
$(TMPDIR)/%.o: %.for
	@echo "   Compiling: $^ ---> $@"
#	@$(F77) -m32 -fPIC -Wno-globals -c $^ -o $@
	@$(F77) -fPIC -Wno-globals -c $^ -o $@

.PHONY: clean view

clean:
	@rm -vf $(TMPDIR)/$(LIBNAME)Dictionary.*
	@rm -vf $(OBJS)
	@rm -vf $(LIBDIR)/lib$(LIBNAME).so
	@rm -vf $(TMPDIR)/howto_Rkin.o
	@rm -vf howto_Rkin
