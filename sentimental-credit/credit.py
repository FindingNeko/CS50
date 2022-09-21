# A program to check if your CC number is real.
ccList = []
ccInt = input("Number: ")
# Try to convert the input to an int.
try:
    ccInt = int(ccInt)
# If that doesn't work, INVALID
except:
    print("INVALID")
# If it does work, carry on to checking for the right numbers.
else:
    # Determine if number provided is in the right range to be a CC number.
    if ccInt in range(4000000000000, 5599999999999999):
        # Add it to the number list if it is.
        for c in str(ccInt):
            ccList.append(int(c))

        # Use Luhn's algorithm to check the validity of the numbers.
        eodSum = 0
        for i in range(2, len(ccList) + 1, 2):
            if ccList[-i] * 2 > 9:
                eodSum -= 9
            eodSum += ccList[-i] * 2
        for i in range(1, len(ccList) + 1, 2):
            eodSum = eodSum + ccList[-i]
        if eodSum % 10 != 0:
            print("INVALID")
        # If it passes Luhn's algorithm, figure out which type of card it is.
        elif (len(ccList) == 13 or len(ccList) == 16) and ccList[0] == 4:
            print("VISA")
        elif (len(ccList) == 15 and ccList[0] == 3 and (ccList[1] == 4
              or ccList[1] == 7):
            print("AMEX")
        elif len(ccList) == 16 and ccList[0] == 5 and int(ccList[1]) < 6:
            print("MASTERCARD")
        # If it doesn't match any of the type patters, assume it is INVALID
        else:
            print("INVALID")
    # If it isn't in the range it is INVALID
    else:
        print("INVALID")
