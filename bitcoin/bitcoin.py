from sys import argv, exit
import requests
import json


def main():
    if len(argv) != 2:
        exit("Usage: python3 bitcoin.py [number]")
    try:
        n = float(argv[1])
    except ValueError:
        exit("Invalid number.")

    total = n * return_price()
    print(f"${total:,.4f}")

def return_price():
    try:
        result = requests.get("https://api.coindesk.com/v1/bpi/currentprice.json")
    except requests.RequestException as e:
        print("Error occured. ", e)

    price = result.json()["bpi"]["USD"]["rate_float"]
    return price

if __name__ == "__main__":
    main()
