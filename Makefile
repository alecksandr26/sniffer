C = clang
CFLAGS = -g -W
CFLAGS_STATIC = -static-libgcc

LIBS = lib/libfile.so lib/libhelpers.so lib/libpackage.so lib/libethernet.so lib/libprotocols.so lib/libnetwork.so

LIBS_STATIC = lib/libfile.o lib/libhelpers.o lib/libpackage.o lib/libethernet.o lib/libprotocols.o lib/libnetwork.o

PROTOCOLS_OBJECTS = lib/arp.o lib/icmp.o lib/icmpv6.o lib/ipv4.o lib/ipv6.o lib/protocolstransport.o

PROTO = src/protocols/arp.c src/protocols/icmp.c src/protocols/icmpv6.c  src/protocols/ipv4.c src/protocols/ipv6.c src/protocols/protocolstransport.c
PROTO_H =  include/protocols/arp.h include/protocols/icmp.h include/protocols/icmpv6.h include/protocols/ipv4.h include/protocols/ipv6.h include/protocols/protocolstransport.h

all: main

# All the shared dependencies
lib/libfile.so: src/helpers/file.c include/helpers/file.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

# To compile the network dependency 
lib/libnetwork.so: src/helpers/network.c include/helpers/file.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

# Some extra usefull functions used by serveral dependencies
lib/libhelpers.so: src/helpers/helpers.c include/helpers/helpers.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

# Here are the libraries 
lib/libpackage.so: src/package.c include/package.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

lib/libethernet.so: src/ethernet.c include/ethernet.h
	$(C) $(CFLAGS) -fPIC -shared -lc $< -o $@

lib/libprotocols.so: $(PROTO) $(PROTO_H)
	$(C) $(CFLAGS) -fPIC -shared -lc $(PROTO) -o $@


# All the static dependecies
lib/libfile.o: src/helpers/file.c include/helpers/file.h
	$(C) $(CFLAGS_STATIC) -c $< -o $@

# To compile the network dependency 
lib/libnetwork.o: src/helpers/network.c include/helpers/file.h
	$(C) $(CFLAGS_STATIC) -c $< -o $@

# Some extra usefull functions used by serveral dependencies
lib/libhelpers.o: src/helpers/helpers.c include/helpers/helpers.h
	$(C) $(CFLAGS_STATIC) -c $< -o $@

# Here are the libraries 
lib/libpackage.o: src/package.c include/package.h
	$(C) $(CFLAGS_STATIC) -c $< -o $@

lib/libethernet.o: src/ethernet.c include/ethernet.h
	$(C) $(CFLAGS_STATIC) -c $< -o $@

# Reallocate all the .o files into a library
lib/libprotocols.o: $(PROTO) $(PROTO_H)
	$(C) $(CFLAGS_STATIC) -c $(PROTO)
	mv *.o lib/
	ld -r $(PROTOCOLS_OBJECTS) -o $@

# To compile the main program we need to add the libraries
main: main.c $(LIBS)
	$(C) $(CFLAGS) $< $(LIBS) -o $@ -lpcap

# To compile the binary in a static way
static: main.c $(LIBS_STATIC)
	$(C) $(CFLAGS_STATIC) $< $(LIBS_STATIC) -o $@ -lpcap

clean:
	rm $(LIBS)
	rm main

static_clean:
	rm lib/*.o
	rm static

