from sys import exit, argv

def main():
    if not invalid_input():
        with open("lines.out", "a") as f:
            f.write(f"{argv[1]} has {numberOfLines()} lines. \n")
        print(numberOfLines())


def numberOfLines():
    with open(argv[1], "r") as f:
        lines = f.readlines()
        if len(lines) == 0:
            return 0
        invalid_lines = 0
        for line in lines:
            if (line.startswith("#") or line.lstrip().startswith(" ") or
                line.startswith("\n") or line.startswith('"')):
                invalid_lines += 1
        return len(lines) - invalid_lines


def invalid_input():
    if len(argv) != 2:
        exit("Usage: python lines.py [PATH]")

    filename = argv[1]
    if filename.split(".")[1] != "py":
        exit("Not a valid Python file.")

    try:
        with open(filename, "r") as file:
            pass
    except FileNotFoundError:
        exit("The file does not exist.")

if __name__ == "__main__":
    main()
