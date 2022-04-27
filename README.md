# Sniffer
A program that can interpret network packages by capturing them and analyzing them.

## To build
To build the program run the command `make`.
```
make
```
Or if you want a static binary executable run this another one.
```
make static
```

## To usage
To read captured packages as binary file, run this command<br />
```
./main --file <file-name>
```
Or....
```
./main -f <file-name>
```
<br />
To read a package from the network you will need to run the program with sudo privileges.

```
sudo ./main --network
```

Or...

```
sudo ./main -n
```
## To clean
You can delete all the ".o" or ".so" files running this command depending how do you compile the program.<br />
So if you compile the program as shared linked executable run this command.
```
make clean
```
Or if you compile the program as static executable run this command.
```
make clean_static
```

## Todo
Create the tcp and udp interpretation
