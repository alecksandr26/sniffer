C = clang
CFLAGS = -g -W

LIBS = lib/libfile.so lib/libhelpers.so lib/libpackage.so lib/libethernet.so lib/libprotocols.so

PROTO = src/protocols/*.c
PROTO_H = include/protocols/*.h

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

lib/libprotocols.so: $(PROTO) $(PROTO_H)
	$(C) $(CFLAGS) -fPIC -shared -lc $(PROTO) -o $@ 

# To compile the main program 
main: main.c $(LIBS)
	$(C) $(CFLAGS) $< $(LIBS) -o $@

clean:
	rm $(BINS) $(LIBS)
