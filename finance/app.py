import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Create a global purchase dictionary to carry data from /buy to /confirm
purchase = {"stock": "", "shares": 0, "unit_price": 0, "net_price": 0}

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

@app.route("/")
@login_required
def index():
    user_cash = db.execute("SELECT cash FROM users WHERE id=?;", session['user_id'])[0]["cash"]

    def portfolio_balance():
        # Find balance by adding each of the stock's current price (via the API) times the sum of all the shares ever bought by the user
        value = 0
        rows = db.execute(f"SELECT stock AS stock_symbol, SUM(shares) AS share_num FROM history JOIN users ON history.user_id = users.id WHERE users.id = ? GROUP BY stock;", session["user_id"])
        for row in rows:
            value_shares = "{:.2f}".format(lookup(row["stock_symbol"])["price"] * int(row["share_num"]))
            value = value + float(value_shares)
        return value

    def table_data():
        STOCK_LIST = db.execute(f"SELECT stock, COUNT(stock) AS purchase_num, SUM(shares) as share_num FROM history JOIN users ON history.user_id = users.id WHERE users.id = ? GROUP BY stock;", session['user_id'])
        print(STOCK_LIST)
        CURRENT_STOCK_INFO = dict()
        #csp: current share price; ctv: current total value
        for stock in STOCK_LIST:
            # Retrieve real-time share prices to compute ctv and append to the every dictionary in the queried list a dict
            # with the real-time info.
            csp = lookup(stock["stock"])["price"]
            CURRENT_STOCK_INFO["csp"] = "{:.2f}".format(csp)
            CURRENT_STOCK_INFO["ctv"] = "{:.2f}".format(csp * int(stock["share_num"]))
            stock.update(CURRENT_STOCK_INFO)
        return STOCK_LIST

    # Check if user has any stocks
    if db.execute(f"SELECT COUNT(*) FROM history JOIN users ON history.user_id = users.id WHERE users.id = ?;", session['user_id'])[0]["COUNT(*)"] == 0:
        return render_template("index_empty.html")
    else:
        balance= portfolio_balance()
        return render_template("index.html", cash="{:.2f}".format(user_cash), grand_balance="{:.2f}".format(user_cash + balance),
                           stock_list=table_data(), balance="{:.2f}".format(balance))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        user_cash = db.execute("SELECT cash FROM users WHERE id=?;", session['user_id'])
        return render_template("buy.html", cash="{:.2f}".format(user_cash[0]["cash"]))
    else:
        # Verfiy stock symbol is not empty/invalid. Notice the stock dictionary is local to this function, while purchase global.
        stock = lookup(request.form.get("symbol"))
        if stock is None:
            return apology("Invalid stock symbol")

        # Verify valid # of shares and enough cash. If not, apologize; else, show confirmation page
        net_price = 0
        user_cash = 0
        if float(request.form.get("shares")) <= 0:
            return apology("Invalid number of shares")
        else:
            net_price = float(request.form.get("shares")) * stock["price"]
            user_cash = db.execute("SELECT cash FROM users WHERE id=?;", session['user_id'])[0]["cash"]

        if net_price > user_cash:
            return apology("You do not have enough funds")
        else:
            # Make the purchase information available to the confirm function. float user's share input (precaution).
            global purchase
            purchase["stock"] = request.form.get("symbol")
            purchase["shares"] = float(request.form.get("shares"))
            purchase["unit_price"] = stock["price"]
            purchase["net_price"] = net_price
            return render_template("confirm.html", symbol=stock["symbol"], shares=purchase["shares"], unit_price=stock["price"], net_price="{:.2f}".format(net_price))

@app.route("/confirm", methods=["GET", "POST"])
@login_required
def confirm():

    # Confirm purchase into database and update user's cash
    db.execute("INSERT INTO history (user_id, stock, shares, unit_price, net_price) VALUES(?, ?, ?, ?, ?);", session['user_id'], purchase['stock'], purchase['shares'], purchase['unit_price'], purchase['net_price'])
    db.execute("UPDATE users SET cash=cash - ? WHERE id=?;", purchase["net_price"], session['user_id'])
    return redirect("/")

@app.route("/history")
@login_required
def history():
    # return dictionary list for purchases made by user currently logged in.
    purchases = db.execute("SELECT stock, shares, unit_price AS share_price, net_price FROM history WHERE user_id=?", session["user_id"])
    sales = db.execute("SELECT stock, shares, unit_price AS share_price, profit FROM sales WHERE user_id=?", session["user_id"])
    # Format numbers to two decimal places correctly
    for p in purchases:
        p['shares'] = "{:.2f}".format(p['shares'])
        p['share_price'] = "{:.2f}".format(p['share_price'])
        p['net_price'] = "{:.2f}".format(p['net_price'])
    for s in sales:
        s['shares'] = "{:.2f}".format(s['shares'])
        s['share_price'] = "{:.2f}".format(s['share_price'])
        s['profit'] = "{:.2f}".format(s['profit'])
    return render_template("history.html", purchase_list=purchases, sales_list=sales)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    # Check request method; POST means user submitted the quote form; GET means user wants to quote
    if request.method == "GET":
        return render_template("quote.html")
    elif request.method == "POST":
        quote_dict = lookup(request.form.get("symbol"))
        # Verify user enter valid symbol
        if (quote_dict is None):
            return apology("Symbol was not found")
        else:
            return render_template("quoted.html", name=quote_dict["name"], symbol=quote_dict["symbol"], price=quote_dict["price"])


@app.route("/register", methods=["GET", "POST"])
def register():
    # Check for request method
    if request.method == "GET":
        return render_template("register.html")
    else:
        def confirm_pw():
            if request.form.get("password") == request.form.get("confirmation"):
                return True
            else:
                return False
        # Verify if username is not empty before querying database
        username_exists = False
        if request.form.get("username") == "":
            return apology("Invalid username")
        else:
            username_exists = True
        if username_exists:
            # Query database
            USERNAMES_IN_DB = db.execute("SELECT COUNT(*) FROM users WHERE username = ?;", request.form.get("username"))
            if USERNAMES_IN_DB[0]["COUNT(*)"] > 0:
                return apology("Username already taken")
            elif request.form.get("password") == "" or not confirm_pw():
                return apology("Passwords do not match")
            else:
                # Everything was successful, insert user into datbase
                db.execute("INSERT INTO users (username, hash) VALUES(?, ?);", request.form.get("username"),
                        generate_password_hash(request.form.get("password")))
                return redirect("/login")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        stocks_db = db.execute("SELECT DISTINCT stock FROM history JOIN users ON history.user_id = users.id WHERE users.id=?;", session['user_id'])
        if len(stocks_db) == 0:
            return render_template("sell_empty.html")
        else:
            stocks_owned = list()
            for row in stocks_db:
                stocks_owned.append(row["stock"])
            return render_template("sell.html", stocks_owned=stocks_owned)
    elif request.method == "POST":
        # Extract user's input
        shares_sold = request.form.get("shares")
        stock_sold = request.form.get("symbol")
        shares_owned = db.execute(f"SELECT SUM(shares) as shares_total FROM history WHERE user_id=? AND stock=? GROUP BY stock;", session['user_id'], stock_sold)[0]["shares_total"]
        if float(shares_sold) > float(shares_owned):
            return apology("You do not own sufficient shares.")
        else:
            # Find the larget share purchase in history, and subtract from it the sold shares.
            max_share_id = db.execute("SELECT id FROM history WHERE shares = (SELECT MAX(shares) FROM history WHERE stock = ? AND user_id = ?);", stock_sold, session["user_id"])
            db.execute(f"UPDATE history SET shares = shares - ? WHERE id=?;", shares_sold, max_share_id[0]['id'])
            # Increase user's cash
            unit_price = lookup(stock_sold)["price"]
            profit = float(shares_sold) * float(unit_price)
            db.execute(f"UPDATE users SET cash = cash + ? WHERE id=?", profit, session["user_id"])
            # Log in database
            db.execute(f"INSERT INTO sales (user_id, stock, shares, unit_price, profit) VALUES(?, ?, ?, ?, ?);", session["user_id"], stock_sold, shares_sold, unit_price, profit)
        return redirect("/")

