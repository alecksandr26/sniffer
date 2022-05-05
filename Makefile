C = clang
CFLAGS = -g -W
CFLAGS_STATIC = -static-libgcc
STATIC=false

LIBS = lib/libfile.so lib/libhelpers.so lib/libpackage.so lib/libethernet.so lib/libprotocols.so lib/libnetwork.so

LIBS_STATIC = lib/libfile.o lib/libhelpers.o lib/libpackage.o lib/libethernet.o lib/libprotocols.o lib/libnetwork.o

PROTOCOLS_OBJECTS = lib/arp.o lib/icmp.o lib/icmpv6.o lib/ipv4.o lib/ipv6.o lib/protocolstransport.o

PROTO = src/protocols/arp.c src/protocols/icmp.c src/protocols/icmpv6.c  src/protocols/ipv4.c src/protocols/ipv6.c src/protocols/protocolstransport.c src/protocols/udp.c src/protocols/protocolsAplication.c src/protocols/dns.c
PROTO_H =  include/protocols/arp.h include/protocols/icmp.h include/protocols/icmpv6.h include/protocols/ipv4.h include/protocols/ipv6.h include/protocols/protocolstransport.h include/protocols/udp.h include/protocols/protocolsAplication.h include/protocols/dns.h

# List of all the tests
TESTS = test/testipv6package


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

# To create the directory
lib:
	mkdir $@


# To compile the main program we need to add the libraries
main: main.c lib $(LIBS)
	$(C) $(CFLAGS) $< $(LIBS) -o $@ -lpcap


# To compile the binary in a static way
static: main.c lib $(LIBS_STATIC)
	$(C) $(CFLAGS_STATIC) $< $(LIBS_STATIC) -o $@ -lpcap


clean:
	rm $(LIBS) main

clean_static:
	rm $(PROTOCOLS_OBJECTS) $(LIBS_STATIC) static

# This are the tests

# Compile the test 
test/testipv6package: test/testipv6package.c lib/libprotocols.so lib/libhelpers.so
	$(C) $(CFLAGS) $< lib/libprotocols.so lib/libhelpers.so -o $@

# Run all the tests
test: $(TESTS)
	./test/testipv6package


