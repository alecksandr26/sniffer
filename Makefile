C = clang
CFLAGS = -g -W

BINS = main
LIBS = lib/libpackage.so lib/libfile.so lib/libethernet.so lib/libarp.so lib/libipv4.so
SUBLIBS = lib/ethernet/libethernet.o lib/ethernet/libpethernet.o

all: $(BINS)

# Here are the libraries 
lib/libpackage.so: src/package.c include/package.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

lib/libfile.so: src/file.c include/file.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

# To compile all the ethernet module 
lib/ethernet/libethernet.o: src/ethernet/ethernet.c include/ethernet/ethernet.h
	$(C) $(CFLAGS) -fPIC -c $< -o $@

lib/ethernet/libpethernet.o: src/ethernet/pethernet.c include/ethernet/pethernet.h
	$(C) $(CLFAGS) -fPIC -c $< -o $@


# We create the module of ethernet 
lib/libethernet.so: $(SUBLIBS)
	$(C) $(CFLAGS) -fPIC -shared -lc $^ -o $@

# These are all the dependencys for each protocol 
lib/libarp.so: src/protocols/arp.c include/protocols/arp.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

lib/libipv4.so: src/protocols/ipv4.c include/protocols/ipv4.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

# To compile the main program 
main: main.c $(LIBS)
	$(C) $(CFLAGS) $< $(LIBS) -o $@

clean:
	rm $(BINS) $(LIBS)


