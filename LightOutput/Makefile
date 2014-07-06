# Name of shared library
LIBNAME = LightOutput

# All temporary files are placed here,
# such as cint dictionaries and object files.
TMPDIR = ../tmp

# All libraries are placed here.
LIBDIR = ../lib

# All header files are linked here.
HDRDIR = ../include

# If a file that is listed as a prerequisite does not exist in the current directory,
# 'make' searches the directories listed in VPATH for a file with that name.
VPATH = $(TMPDIR)

MYINCLUDES = -I../$(LIBNAME)

CXXFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags) -fPIC -Wall
CXXFLAGS += $(MYINCLUDES)
LDFLAGS += $(shell $(ROOTSYS)/bin/root-config --glibs) -Wall
#LDFLAGS += -m32

HDRS = $(LIBNAME).h
OBJS = $(TMPDIR)/$(LIBNAME).o $(TMPDIR)/$(LIBNAME)Dictionary.o

# Creating shared object library.
$(LIBDIR)/lib$(LIBNAME).so: $(OBJS)
	@echo "   Creating Shared Lib: $^ ---> $@"
	@$(CXX) $(LDFLAGS) -shared -o $@ $^
	@for hdr in $(HDRS); do (cd $(HDRDIR); ln -s ../$(LIBNAME)/$$hdr); done

# Compiling all source files, including dictionary source file, using a pattern rule.
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

.PHONY: clean

clean:
	@rm -vf $(TMPDIR)/$(LIBNAME)Dictionary.*
	@rm -vf $(OBJS)
	@rm -vf $(LIBDIR)/lib$(LIBNAME).so

view:
	@echo "VPATH: $(VPATH)"
	@echo ""
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo ""
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo ""
	@echo "HDRS: $(HDRS)"
	@echo ""
	@echo "OBJS: $(OBJS)"

