import regex

def main():
    word = input("Input: ")
    print(shorten(word))

def shorten(word):
    return regex.sub('[aeiouAEIOU]', '', word)

if __name__ == "__main__":
    main()
