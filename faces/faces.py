def convert(string):
    return string.replace(':)', '🙂').replace(':(', '🙁')

def main():
    text = input("Text: ")
    print(convert(text))
main()