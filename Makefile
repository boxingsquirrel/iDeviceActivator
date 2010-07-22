CC=gcc
CP=cp
RM=rm -rf
MKDIR=mkdir

CFLAGS=-std=gnu99 `pkg-config --cflags gtk+-2.0`
LFLAGS=-limobiledevice -lplist -lcurl `pkg-config --libs gtk+-2.0` -lirecovery -lzip

SRCDIR=src
RESDIR=res

SRCFILES=$(SRCDIR)/gtk-activator.c $(SRCDIR)/device.c $(SRCDIR)/file.c $(SRCDIR)/ui.c $(SRCDIR)/activate.c
OBJFILES=$(subst .c,.o,$(SRCFILES))
MENUFILE=iDeviceActivator.desktop

OUTFILE=iDeviceActivator

INSTPATH=/usr/local/bin
DATAPATH=/usr/local/share/iDeviceActivator

all: tool
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

tool: $(OBJFILES)
	$(CC) -o $(OUTFILE) $(OBJFILES) $(LFLAGS)
	gtk-builder-convert $(SRCDIR)/ui.glade $(RESDIR)/ui.xml
	gtk-builder-convert $(SRCDIR)/name_prompt.glade $(RESDIR)/name_prompt.xml
	@echo ""
	@echo "Run 'make install' as root to install"

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
