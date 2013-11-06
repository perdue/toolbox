SUBDIRS = Analyzer LightOutput Utility Rkin IsfConverter

.PHONY : all clean

all:
	@for dir in $(SUBDIRS); do (echo ""; echo "lib$$dir.so"; cd $$dir; $(MAKE)); done
#	@echo ""; echo "rkin"; cd rkin; $(MAKE)

clean:
	@for dir in $(SUBDIRS); do (cd $$dir; $(MAKE) clean); done
	@cd rkin; $(MAKE) clean
