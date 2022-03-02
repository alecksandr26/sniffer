C = clang
CFLAGS = -g -W

LIBS = lib/libpackage.so lib/libfile.so lib/libethernet.so lib/protocols.so lib/libhelpers.so

PROTO = src/protocols/arp.c src/protocols/ipv4.c src/protocols/icmp.c
PROTO_H = include/protocols/arp.h include/protocols/ipv4.h include/protocols/icmp.h

all: main

lib/libfile.so: src/file.c include/file.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

# Some extra usefull functions used by serveral dependencies
lib/libhelpers.so: src/helpers.c include/helpers.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

# Here are the libraries 
lib/libpackage.so: src/package.c include/package.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

lib/libethernet.so: src/ethernet.c include/ethernet.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

lib/protocols.so: $(PROTO) $(PROTO_H)
	$(C) $(CFLAGS) -fPIC -shared -lc $(PROTO) -o $@ 

# To compile the main program 
main: main.c $(LIBS)
	$(C) $(CFLAGS) $< $(LIBS) -o $@

clean:
	rm $(BINS) $(LIBS)
