# Mario but easy as py

# Import get_int per instructions to make things interesting
from cs50 import get_int

# Get user input using get_int to determine block height
blocks = 0
while blocks < 1 or blocks > 8:
    blocks = get_int("Height: ")

# For loop to print the correct number of blocks
for i in range(1, blocks + 1):

    print(" " * (blocks - i), end="")
    print("#" * i, end="")
    print("  ", end="")
    print("#" * i, end="")
    print("")
