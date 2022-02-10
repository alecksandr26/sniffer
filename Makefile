C = clang
CFLAGS = -g -W

BINS = main
LIBS = lib/libpackage.so lib/libfile.so lib/libethernet.so lib/libarp.so
SUBLIBS = lib/ethernet/libethernet.o lib/ethernet/libpethernet.o

all: $(BINS)

# Here are the libraries 
lib/libpackage.so: include/package.h src/package.c
	$(C) $(CFLAGS) -fPIC -shared -lc src/package.c -o lib/libpackage.so

lib/libfile.so: include/file.h src/file.c
	$(C) $(CFLAGS) -fPIC -shared -lc src/file.c -o lib/libfile.so


# To compile all the ethernet module 
lib/ethernet/libethernet.o: include/ethernet/ethernet.h src/ethernet/ethernet.c
	$(C) $(CFLAGS) -fPIC -c src/ethernet/ethernet.c -o lib/ethernet/libethernet.o

lib/ethernet/libpethernet.o: include/ethernet/pethernet.h src/ethernet/pethernet.c
	$(C) $(CLFAGS) -fPIC -c src/ethernet/pethernet.c -o lib/ethernet/libpethernet.o


# We create the module of ethernet 
lib/libethernet.so: $(SUBLIBS)
	$(C) $(CFLAGS) -fPIC -shared -lc $(SUBLIBS) -o lib/libethernet.so


lib/libarp.so: include/protocols/arp.h src/protocols/arp.c
	$(C) $(CFLAGS) -fPIC -shared -lc src/protocols/arp.c -o lib/libarp.so

# To compile the main program 
main: main.c $(LIBS)
	$(C) $(CFLAGS) main.c $(LIBS) -o main

clean:
	rm $(BINS) $(LIBS)


