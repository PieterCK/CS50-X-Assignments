import os
import re
from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        if not request.form.get("name") or not request.form.get("birthdate"):
            return render_template("index.html", fail = "Please Complete All Info Before Submitting.")

        name = request.form.get("name")
        birthdate =  re.split("[-]", request.form.get("birthdate"))

        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, birthdate[2], birthdate[1])
        #return render_template("index.html", fail =)
        return redirect("/")

    else:
        # TODO: Display the entries in the database on index.html
        bday = db.execute("SELECT* FROM birthdays")
        return render_template("index.html", bday = bday)

@app.route("/deregister", methods=["POST"])
def deregister():
    id = request.form.get("id")
    if id:
        db.execute("DELETE FROM birthdays WHERE id = ?", id)
    return redirect("/")
