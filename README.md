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
To read captured packages as binary file, run this command using the flag `-f`.
```
./main --file <file-name>
```
```
./main -f <file-name>
```
<br />
To read a packet from the network, use the flag `-n`, also you will need to run the program as root user.
```
sudo ./main --network
```
```
sudo ./main -n
```
### To capture and save network packages
You can capture and save any cached network packet using the flag `-c`.
```
sudo ./main --network --capture
```
```
sudo ./main -n -c
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
1. Start witht the tcp protocol
2. Do a refactor of the complete code

