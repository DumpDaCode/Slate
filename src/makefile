LIBS=`pkg-config --libs gtk+-3.0`
CFLAGS=`pkg-config --cflags gtk+-3.0`
TARGET=slate
RESOURCE=ui.gresource.xml
OBJS=app.o panel_events.o sheet_events.o app_ds.o graphics.o ui.o

.PHONY=all

$(TARGET): all clear

all: res install

res:
	glib-compile-resources --manual-register --sourcedir=../res/ --generate-source $(RESOURCE)
	glib-compile-resources --manual-register --sourcedir=../res/ --generate-header $(RESOURCE)

install: $(OBJS)
	gcc $(OBJS) $(LIBS) -o ../release/slate -lm -rdynamic -no-pie

$(TARGET)-windows: all-windows clear

all-windows: res install-windows

install-windows: $(OBJS)
	gcc $(OBJS) $(LIBS) -o ../release/slate -lm -Wl,--export-all-symbols,--subsystem,windows

%.o: %.c
	gcc -c $(CFLAGS) $^ -o $@ -Wall

clear:
	rm -rf *.o
