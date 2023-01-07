import os
import re

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import date
from decimal import *

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
    id = session["user_id"]
    owned = db.execute("SELECT * FROM stonks WHERE person_id = ?", id)
    cash = db.execute("SELECT cash, username FROM users WHERE id= ?", id)

    list=[]
    equity=0
    for i in range(len(owned)):
        stock = lookup(owned[i]["stock"])
        float = round(stock["price"]* owned[i]["quantity"])

        list.append(stock["name"])
        equity=equity+float

    #New Users' Route for ("/")
    if len(owned) < 1:
        compiledinf=None
        profile ={
            "equity":cash[0]["cash"],
            "cash":cash[0]["cash"],
            "username": cash[0]["username"],
            "float": 0,
            "list":None
        }
        return render_template("index.html", profile=profile)

    #Users' Main Route for ("/")
    else:
        compiledinf=[]
        equity= cash[0]["cash"] + equity
        profile={
            "equity": equity,
            "username": cash[0]["username"],
            "cash": cash[0]["cash"],
            "float": equity-cash[0]["cash"]
        }

        for i in range(len(owned)):
            s_info= lookup(owned[i]["stock"])
            tmp = {
                "symbol": owned[i]["stock"],
                "company": s_info["name"],
                "c_price": s_info["price"],
                "b_price": owned[i]["buy_price"],
                "shares": int(owned[i]["quantity"]),
                "float": s_info["price"] * int(owned[i]["quantity"]),
                "id": owned[i]["id"],
            }
            compiledinf.append(tmp)
        return render_template("index.html", compiledinf=compiledinf, profile=profile)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol") or not request.form.get("shares"):
                return apology("Please Submit Input")

        symbol = request.form.get("symbol")
        share = (request.form.get("shares"))
        check = share.isdecimal()
        if check != True:
            return apology("Please Submit Valid Input")

        if Decimal(share) < 1:
            return apology("Please Submit Valid Input")

        if lookup(symbol) is None:
                return apology("Please Submit Valid Input")
        else:
            s_info=lookup(symbol)

        #Check User Balance
        id = session["user_id"]
        balance = db.execute("SELECT cash FROM users WHERE id = ? LIMIT 1" , id)

        #Check Viability of Transaction
        total = int(share) * s_info["price"]
        if total >balance[0]["cash"]:
            return apology("Sorry, Your Balance is Insufficient")
        else:
            charged= balance[0]["cash"] - total
            db.execute("INSERT INTO stonks (person_id, stock, buy_price, quantity) VALUES(?,?,?,?)", id, symbol,s_info["price"], share)
            db.execute("UPDATE users SET cash = ? WHERE id = ?", charged, id)

        #Updates "History"
        db.execute("INSERT INTO history (person_id, date, price, quantity, type, symbol) VALUES(?,?,?,?,?,?)", id, date.today(), s_info["price"], share, "BUY", symbol)

        return redirect("/")
    else:
        id = session["user_id"]
        owned = db.execute("SELECT  cash, buy_price, quantity, stock, username FROM stonks JOIN users ON  stonks.person_id= users.id WHERE person_id = ?", id)

        list=[]
        equity=0
        for i in range(len(owned)):
            stock = lookup(owned[i]["stock"])
            float = stock["price"]*owned[i]["quantity"]

            list.append(stock["name"])
            equity=equity+float

        if len(owned) < 1:
            cash = db.execute("SELECT cash FROM users WHERE id= ?", id)
            compiledinf ={
                "equity":cash[0]["cash"],
                "cash":cash[0]["cash"],
                "list":None
            }
        else:
            equity= owned[0]["cash"] + equity
            compiledinf ={
                "equity":equity,
                "cash":owned[0]["cash"],
                "list":set(list)
            }
        return render_template("buy.html", compiledinf=compiledinf)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    id = session["user_id"]
    owned = db.execute("SELECT * FROM history WHERE person_id = ?", id)

    #New Users' Route for ("/History")
    if len(owned) < 1:
        compiledinf = {
                "symbol": owned[i]["symbol"],
                "company": s_info["name"],
                "price": owned[i]["price"],
                "shares": int(owned[i]["quantity"]),
                "date": owned[i]["date"],
                "type": owned[i]["type"],
            }
        return render_template("history.html", compiledinf=compiledinf)

    #Users' Main Route for ("/History")
    else:
        compiledinf=[]
        for i in range(len(owned)):
            s_info= lookup(owned[i]["symbol"])
            tmp = {
                "symbol": owned[i]["symbol"],
                "company": s_info["name"],
                "price": owned[i]["price"],
                "shares": int(owned[i]["quantity"]),
                "date": owned[i]["date"],
                "type": owned[i]["type"],
            }
            compiledinf.append(tmp)
        return render_template("history.html", compiledinf=compiledinf)


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
                return apology("Please Submit Input")

        stonks= request.form.get("symbol")

        if lookup(stonks) is None:
                return apology("Please Submit Valid Input")
        else:
            s_info=lookup(stonks)
            s_info["price"]=int(s_info["price"])
            return render_template("quote.html",s_info=s_info)
    else:
        s_info={
            "name":None,
            "symbol":None,
            "price":0
        }
        return render_template("quote.html",s_info=s_info)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        if not request.form.get("username") or not request.form.get("confirmation") or not request.form.get("password"):
            return apology("Incomplete Information")
        username = request.form.get("username")
        if request.form.get("confirmation") != request.form.get("password"):
            return apology("Password Doesn't Match")

        user_list = db.execute("SELECT username FROM users")
        for i in range(len(user_list)):
            if username == user_list[i]["username"]:
                return apology("Username has already been taken")

        hash = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=16)

        #Inserts Newly Registered Users' Account Infos
        db.execute("INSERT INTO users (username, hash, email) VALUES(?,?,?)", username, hash, "email@gmail.com")

        # Log in Newly Registered User
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        session["user_id"] = rows[0]["id"]
        return redirect("/")
    else:
        return render_template("register.html",fail = "Welcome, New User!")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("Please Submit Input")
        symbol = request.form.get("symbol")
        share = int(request.form.get("shares"))
        id = session["user_id"]
        s_owned = db.execute("SELECT quantity, id FROM stonks WHERE person_id = ? AND stock=?", id, symbol)
        owned_q = db.execute("SELECT SUM(quantity)AS sum FROM stonks WHERE person_id = ? AND stock = ?", id, symbol)

        if int(share) > owned_q[0]["sum"]:
            return apology("Insufficient Stock")
        else:


            # Updates "Users Cash"
            s_info=lookup(symbol)
            value = int(share) * s_info["price"]
            cash = db.execute("SELECT cash FROM users WHERE id = ?", id)
            n_cash = cash[0]["cash"] + value
            db.execute("UPDATE users SET cash=? WHERE id = ?", n_cash, id)

            # Updates "History"
            db.execute("INSERT INTO history (person_id, date, price, quantity, type, symbol) VALUES(?,?,?,?,?,?)", id, date.today(), s_info["price"], share, "SELL", symbol)

            # Updates "Stonks"
            while share > 0:
                for i in range(len(s_owned)):
                    if share > s_owned[i]["quantity"]:
                        share = share - s_owned[i]["quantity"]
                        db.execute("DELETE FROM stonks WHERE id = ?", s_owned[i]["id"])
                    elif share == s_owned[i]["quantity"]:
                        share = share - s_owned[i]["quantity"]
                        db.execute("DELETE FROM stonks WHERE id = ?", s_owned[i]["id"])
                    else:
                        tmp = s_owned[i]["quantity"] - share
                        db.execute("UPDATE stonks SET quantity = ? WHERE id = ?", tmp, s_owned[i]["id"])
                        share = 0
            return redirect("/")

    # Get Method
    elif request.method == "GET":
        id = session["user_id"]
        owned = db.execute("SELECT * FROM stonks WHERE person_id = ?", id)
        cash = db.execute("SELECT cash, username FROM users WHERE id= ?", id)

        list=[]
        equity=0
        for i in range(len(owned)):
            stock = lookup(owned[i]["stock"])
            float = round(stock["price"]* owned[i]["quantity"])

            list.append(stock["name"])
            equity=equity+float

        #New Users' Route for ("/sell")
        if len(owned) < 1:
            return apology("Sorry, No Stonks to Sell")

        #Users' Main Route for ("/sell")
        else:
            compiledinf=[]
            equity= cash[0]["cash"] + equity
            profile={
                "equity": equity,
                "username": cash[0]["username"],
                "cash": cash[0]["cash"],
                "float": equity-cash[0]["cash"]
            }

            for i in range(len(owned)):
                s_info= lookup(owned[i]["stock"])
                tmp = {
                    "symbol": owned[i]["stock"],
                    "company": s_info["name"],
                    "c_price": s_info["price"],
                    "b_price": owned[i]["buy_price"],
                    "shares": int(owned[i]["quantity"]),
                    "float": s_info["price"] * int(owned[i]["quantity"]),
                    "id": owned[i]["id"],
                }
                compiledinf.append(tmp)
            return render_template("sell.html", compiledinf=compiledinf, profile=profile)

