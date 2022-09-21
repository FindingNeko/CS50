import csv
import sys


def main():
    # Check for command line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        return 1
    # Assign command line aruments to variables.
    dnaDB = sys.argv[1]
    dnaF = sys.argv[2]
    # Initialize a list to hold rows from a dna database.
    dnaDbList = []
    # Read database file into the list variable.
    with open(dnaDB, 'r') as db:
        dnareader = csv.reader(db)
        for row in dnareader:
            dnaDbList.append(row)

    # Read DNA sequence file into a variable
    with open(dnaF, 'r') as f:
        dnareader = csv.reader(f)
        for row in dnareader:
            dna = row

    # Find longest match of each STR, and check database for matching profiles
    matches = [0] * len(dnaDbList)
    for i in range(1, len(dnaDbList)):
        for j in range(1, len(dnaDbList[i])):
            if longest_match(dna[0], dnaDbList[0][j]) != int(dnaDbList[i][j]):
                continue
            else:
                # Match found, increment counter in the matches list.
                matches[i] += 1
                # If matches counter indicates all strings match, it's a match.
                if matches[i] == len(dnaDbList[0]) - 1:
                    print(dnaDbList[i][0])
                    return
    # If no match is found, print No match and return
    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
