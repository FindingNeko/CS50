import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


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
    """Show portfolio of stocks"""
#   Get from DB:
#       users -> cash
    user = db.execute("SELECT username, cash FROM users WHERE id=?", session["user_id"])
    name = user[0]["username"]
    cash = user[0]["cash"]
#       transactions -> sum(shares) group by symbol
    shares = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])
    trans = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])
    total = 0
    for x in shares:
        print(x["symbol"])
        quote = lookup(x["symbol"])
        if not quote:
            continue
        total += quote.get("price") * x["SUM(shares)"]
        x["total"] = float(total)
        x["cprice"] = float(quote.get("price"))
    total += cash

#   Calculate:
#       stock holdings
#       total balance including stocks and cash using current quotes

    return render_template("index.html", name=name, cash=cash, total=total, shares=shares, trans=trans)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        cash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])
        username = db.execute("SELECT username FROM users WHERE id=?", session["user_id"])
        username = username[0]["username"]
        quote = lookup(request.form.get("symbol"))
        if not quote:
            return apology("stock symbol does not exist")
        symbol = quote.get("symbol")
        price = quote.get("price")
        shares = request.form.get("shares")
        try:
            shares = int(shares)
        except:
            return apology("Please enter an integer")
        if shares < 1:
            return apology("Please enter a positive integer")
        subt = shares * price
        cash = float(cash[0]["cash"])
        if cash >= subt:
            tota = cash - subt
            db.execute("INSERT INTO transactions (user_id, symbol, price, shares, time) VALUES (?, ?, ?, ?, ?)",
                       session["user_id"], symbol, price, shares, datetime.now())
            db.execute("UPDATE users SET cash = ? WHERE id = ?", tota, session["user_id"])
            return redirect("/")
        else:
            return apology("insufficient funds")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    trans = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])
    for x in trans:
        if x["shares"] < 0:
            x["sell"] = "1"
    return render_template("history.html", trans=trans)


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
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol")
        quote = lookup(request.form.get("symbol"))
        if not quote:
            return apology("Symbol not found", 400)
        return render_template("quoted.html", name=quote.get("name"), price=quote.get("price"), symbol=quote.get("symbol"))
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must provide username", 400)
        elif not request.form.get("password"):
            return apology("must provide password", 400)
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)
        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("passwords do not match", 400)
        else:
            try:
                db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get(
                    "username"), generate_password_hash(request.form.get("password")))
            except:
                return apology("username is in use", 400)
            return render_template("login.html")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    sharesums = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])
    held = {}
    for x in sharesums:
        held.update({x["symbol"]: x["SUM(shares)"]})
    if request.method == "POST":
        symbol = request.form.get("symbol")
        query = lookup(symbol)
        if not query:
            return apology("must provide valid symbol", 400)
        price = query["price"]
        shares = request.form.get("shares")
        try:
            shares = int(shares)
        except:
            return apology("Must enter a whole number of shares", 400)
        if not symbol:
            return apology("must provide valid symbol", 400)
        elif not shares:
            return apology("enter sale quantity", 400)
        elif shares < 1:
            return apology("invlaid sale quantity", 400)
        elif not held.get(symbol) or held.get(symbol) < shares:
            return apology("you do not own enough stock", 400)
        else:
            db.execute("INSERT INTO transactions (user_id, symbol, price, shares, time) VALUES (?, ?, ?, ?, ?)",
                       session["user_id"], symbol, price, -shares, datetime.now())
            cash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])
            cash = cash[0]["cash"]
            cash += price * shares
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session["user_id"])
            return redirect("/")
    else:
        return render_template("sell.html", held=held)