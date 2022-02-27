C = clang
CFLAGS = -g -W

LIBS = lib/libpackage.so lib/libfile.so lib/libethernet.so lib/protocols.so

ETHER = src/ethernet/ethernet.c src/ethernet/pethernet.c
ETHER_H = include/ethernet/ethernet.h include/ethernet/pethernet.h

PROTO = src/protocols/arp.c src/protocols/ipv4.c
PROTO_H = include/protocols/arp.h include/protocols/ipv4.h

all: main

# Here are the libraries 
lib/libpackage.so: src/package.c include/package.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

lib/libfile.so: src/file.c include/file.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

lib/libethernet.so: $(ETHER) $(ETHER_H)
	$(C) $(CFLAGS) -fPIC -shared -lc $(ETHER) -o $@

lib/protocols.so: $(PROTO) $(PROTO_H)
	$(C) $(CFLAGS) -fPIC -shared -lc $(PROTO) -o $@ 

# To compile the main program 
main: main.c $(LIBS)
	$(C) $(CFLAGS) $< $(LIBS) -o $@

clean:
	rm $(BINS) $(LIBS)


