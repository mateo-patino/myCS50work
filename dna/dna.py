import csv
from sys import argv


def main():

    # TODO: Check for command-line usage
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")

    # TODO: Read database file into a variable
    db = open(argv[1], 'r')
    person = csv.DictReader(db)
    STRs = person.fieldnames
    STRs = STRs[1:]

    # TODO: Read DNA sequence file into a variable
    seq = open(argv[2], 'r')
    dna = seq.read()

    # TODO: Find longest match of each STR in DNA sequence
    repeats = dict()
    for s in STRs:
        repeats[s] = longest_match(dna, s)

    # TODO: Check database for matching profiles
    tmp = dict()
    no_match = True
    for dctn in person:
        for seq in STRs:
            tmp[seq] = int(dctn[seq])
        if tmp == repeats:
            print(dctn['name'])
            no_match = False
            break
        else:
            tmp.clear()
    if no_match:
        print('No match')

    close(db)
    close(seq)

# TAKES TWO STRINGS
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
