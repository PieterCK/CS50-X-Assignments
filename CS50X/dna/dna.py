import csv
import sys
import re


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit()
    # TODO: Read database file into a variable
    with open(sys.argv[1], "r") as file:
        database = csv.reader(file)
        for row in database:
            STR = row
            break

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as file:
        sqnc = csv.reader(file)
        for row in sqnc:
            Sequence = row
    matches = {}
    matching(Sequence, STR, 1, matches)
    return


def matching(Sequence, STR, num, matches):
    tmp = longest_match(str(Sequence), STR[num])

    with open(sys.argv[1], "r") as file:
        database = csv.DictReader(file)
        for row in database:
            tmp2 = int(row[STR[num]])
            if tmp == tmp2:
                if row["name"] in matches:
                    matches[row["name"]] += 1
                else:
                    matches[row["name"]] = 1

        num += 1
        if num <= len(STR)-1:
            return matching(Sequence, STR, num, matches)
        elif num > len(STR)-1:
            matched = max(matches, key=matches.get)
            if matches[matched] == (len(STR)-1):
                print(matched)
                exit()

        print("No Match")
        exit()


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
