# Sniffer
Is a program that sniffes the network
## To build
You only need to run the Makefile with the command `make`.
```
make
```
Or if you want to get a portable exectuable.

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
