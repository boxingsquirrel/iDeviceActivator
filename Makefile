CC=gcc
CP=cp
RM=rm -rf
MKDIR=mkdir

CFLAGS=-std=gnu99 `pkg-config --cflags gtk+-2.0`
LFLAGS=-limobiledevice -lplist -lcurl `pkg-config --libs gtk+-2.0` -lzip

SRCDIR=src
RESDIR=res
DATADIR=data

SRCFILES=$(SRCDIR)/gtk-activator.c $(SRCDIR)/device.c $(SRCDIR)/file.c $(SRCDIR)/ui.c $(SRCDIR)/activate.c $(SRCDIR)/ideviceactivate.c $(SRCDIR)/cache.c $(SRCDIR)/util.c
OBJFILES=$(subst .c,.o,$(SRCFILES))
MENUFILE=iDeviceActivator.desktop

OUTFILE=iDeviceActivator
PACKAGE=iDeviceActivator_0.1_i386.deb

INSTPATH=/usr/local/bin
DATAPATH=/usr/local/share/iDeviceActivator

all: dist

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

tool: $(OBJFILES)
	$(CC) -o $(OUTFILE) $(OBJFILES) $(LFLAGS)
	gtk-builder-convert $(SRCDIR)/ui.glade $(RESDIR)/ui.xml
	gtk-builder-convert $(SRCDIR)/name_prompt.glade $(RESDIR)/name_prompt.xml
	@echo ""
	@echo "Run 'make install' as root to install"

deb: tool
	$(RM) dpkg/usr
	$(MKDIR) dpkg/usr
	$(MKDIR) dpkg/usr/local
	$(MKDIR) dpkg/usr/local/bin
	$(MKDIR) dpkg/usr/local/share
	$(MKDIR) dpkg/usr/local/share/iDeviceActivator
	$(MKDIR) dpkg/usr/local/share/iDeviceActivator/res
	$(MKDIR) dpkg/usr/local/share/iDeviceActivator/data
	$(MKDIR) dpkg/usr/share
	$(MKDIR) dpkg/usr/share/applications
	$(CP) $(OUTFILE) dpkg$(INSTPATH)
	$(CP) $(RESDIR)/* dpkg$(DATAPATH)/
	$(CP) $(DATADIR)/* dpkg$(DATAPATH)/
	$(CP) $(MENUFILE) dpkg/usr/share/applications/$(MENUFILE)
	dpkg -b dpkg $(PACKAGE)

source_archive:
	tar -cvzf source.tar.gz data res src CREDITS README Makefile iDeviceActivator.desktop

binary_archive: tool
	tar -cvzf binary.tar.gz data res CREDITS README iDeviceActivator

dist: source_archive binary_archive deb

install:
	@echo "Installing 1 file to "$(INSTPATH)"..."
	$(CP) ./$(OUTFILE) $(INSTPATH)/$(OUTFILE)
	@echo "Installing 4 files to "$(DATAPATH)"..."
	$(RM) $(DATAPATH)
	$(MKDIR) $(DATAPATH)
	$(MKDIR) $(DATAPATH)/res $(DATAPATH)/data
	$(CP) ./res/* $(DATAPATH)/res
	$(CP) ./data/* $(DATAPATH)/data
	@echo "Adding "$(OUTFILE)" to the menu..."
	cp $(MENUFILE) /usr/share/applications/$(MENUFILE)
	@echo "Install complete"
	@echo "To remove run 'make uninstall' as root"

uninstall:
	@echo "Removing 1 file from "$(INSTPATH)"..."
	$(RM) $(INSTPATH)/$(OUTFILE)
	@echo "Removing 4 files from "$(DATAPATH)"..."
	$(RM) $(DATAPATH)
	@echo "Removing "$(OUTFILE)" from the menu..."
	$(RM) /usr/share/applications/$(MENUFILE)
	@echo "Uninstall complete"

clean:
	$(RM) $(OUTFILE) $(OBJFILES)
