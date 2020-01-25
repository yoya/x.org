SHELL = /bin/sh
MAKE = make
NMAKE = nmake
FLAGS = $(MFLAGS) -f Makefile.ini BOOTSTRAPCFLAGS="$(BOOTSTRAPCFLAGS)"

all:
	@echo Please use make World, or on NT use make World.Win32.
	@echo Check the configuration parameters in the ./config/cf directory,
	@echo and pass BOOTSTRAPCFLAGS if necessary.
	@echo Read the release notes carefully before proceeding.
	@echo Do not name your log file make.log or it will be deleted.
	@echo If the top-level Makefile gets corrupted, copy Makefile.ini
	@echo to Makefile and try again.

World:
	$(MAKE) $(FLAGS) World

World.Win32:
	$(NMAKE) $(MFLAGS) -f Makefile.ini World.Win32
