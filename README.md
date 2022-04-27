# Sniffer
Is a program that sniffes the network
## To build
To build the program run the command `make`.
```
make
```
Or if you want a static binary executable run this another one.
```
make static
```

## To usuage
To read captured packages as binary file, run this command<br />
```
./main --file <file-name>
```<br />
Or....<br />
```
./main -f <file-name>
```<br /><br />

To read a package from the network you will need to run the program with sudo privileges.<br />
```
sudo ./main --network
```<br />
Or...<br />
```
sudo ./main -n
```<br />
## Todo
Create the tcp and udp interpretation
