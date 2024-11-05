from cs50 import get_string


# Main function. Returns grade level.
def main():
    txt = get_string("Text: ")
    wrd_lst = txt.split()
    L = letters(wrd_lst)
    S = sentences(txt, wrd_lst)
    grade = index(L, S)

    if grade >= 16:
        print('Grade 16+')
    elif grade < 1:
        print('Before Grade 1')
    else:
        print(f'Grade {grade}')


# Takes in a string and list of words, and returns avg. number of letters per 100 words
def letters(words):
    letters = 0

    # Iterate through each alphabetical character for every word in words, and increase letters by 1
    for word in words:
        for char in word:
            ascii = ord(char.lower())
            if ascii >= 97 and ascii <= 122:
                letters += 1
    l = (letters / len(words)) * 100
    return l


# Takes in a string, and returns avg. number of sentences per 100 words
def sentences(text, words):
    sentences = 0
    for char in text:
        # Use '.', '!' and '?' as sentence terminators.
        if char == '.' or char == '!' or char == '?':
            sentences += 1
    s = (sentences / len(words)) * 100
    return s


def index(L, S):
    # Compute index
    indx = (0.0588 * L) - (0.296 * S) - 15.8
    return round(indx)


main()