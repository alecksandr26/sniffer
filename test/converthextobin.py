import sys
from os.path import exists

# get_hex: return the hex data from a file 
def get_hex(file_name):
    if not exists(file_name):
        print(f"This {file_name} doesn't exist!!!!")
        exit(1)

    f = open(file_name, "r")
    return f.read()

# create_hex_list: It creates a list from the data  and return it
def create_hex_list(data):
    hex_list = []

    # Firs parse a lines
    lines = data.split('\n')
    for line in lines:
        # Second parse the spaces
        hexa = line.split(' ')[3:]
        # Now concatenate all the elements
        hex_list += filter(lambda val : val != ' ', hexa)
    
    return hex_list

# create_binary: Creates the binary file from the list hex
def create_binary(hex_list, bin_name):
    # check the file name
    if exists(bin_name):
        print(f"The file {bin_name} already exist!!!!!")
        exit(1)
        
    f = open(bin_name, "wb") 
    for h in hex_list:
        byte = bytes.fromhex(h)
        # Now write the byte
        f.write(byte)
    # and close the file
    f.close()
    


# The main function 
def main(argv):
    if len(argv) == 1:
        print("I need a file!!!!!")
        exit(1)
    elif len(argv) == 2:
        print("I need a file name where dump the binary")
        exit(1)
    elif len(argv) == 3:
        data = get_hex(argv[1])
        hex_list = create_hex_list(data)
        create_binary(hex_list, argv[2])
        print("Finish")
        exit(0)
    else:
        print("You need to specify the file and also a result file name")
        exit(1)


main(sys.argv)






