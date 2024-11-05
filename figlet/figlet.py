from pyfiglet import Figlet
import random
import sys

figlet = Figlet()
fonts = figlet.getFonts()


if len(sys.argv) == 1:
    figlet.setFont(font=random.choice(fonts))

elif len(sys.argv) == 3:
    if ((sys.argv[1] == "-f") or (sys.argv[1] == "--font")) and (sys.argv[2] in fonts):
        figlet.setFont(font=sys.argv[2])
    else:
        sys.exit("Invalid command or font.")
else:
    sys.exit("Provide either 0 or 2 command-line arguments.")

text = input("Text: ")
print(figlet.renderText(text))
