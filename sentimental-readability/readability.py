from cs50 import get_string

# Use get_string to get an input from the user to analyze.
userstr = get_string("Text: ")

# Initialize counters and flags to use in counting logic.
lcount = 0
wcount = 0
scount = 0
lflag = False
# Iterate over the user input, userstr.
for i in userstr:
    # Check for a letter
    if (ord(i) > 64 and ord(i) < 91) or (ord(i) > 96 and ord(i) < 123):
        lcount += 1
        lflag = True
    # Check for a space after a letter to find a word.
    if ord(i) == 32 and lflag == True:
        wcount += 1
        lflag = False
    # Check for a '!' '.' or '?' after a letter to find a word and sentence.
    if (ord(i) == 33 or ord(i) == 46 or ord(i) == 63) and lflag == True:
        wcount += 1
        scount += 1
        lflag = False
# Calculate the Luhn algortithm grade level.
gradeLevel = 5.88 * lcount / wcount - 29.6 * scount / wcount - 15.8
# Print results, formatted according to specifications.
if gradeLevel < 1:
    print("Before Grade 1")
elif gradeLevel > 16:
    print("Grade 16+")
else:
    print("Grade " + str(round(gradeLevel)))