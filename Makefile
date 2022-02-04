C = clang
CFLAGS = -g -W

BINS = main
LIBS = lib/libpackage.so lib/libfile.so

all: $(BINS)

# Here are the libraries 
lib/libpackage.so: include/package.h src/package.c
	$(C) $(CFLAGS) -fPIC -shared -lc src/package.c -o lib/libpackage.so

lib/libfile.so: include/file.h src/file.c
	$(C) $(CFLAGS) -fPIC -shared -lc src/file.c -o lib/libfile.so


# To compile the main program 
main: main.c $(LIBS)
	$(C) $(CFLAGS) main.c $(LIBS) -o main

clean:
	rm $(BINS) $(LIBS)

