
from flask import Flask, render_template, request, redirect
from cs50 import SQL

app =  Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///moonprints.db")

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

@app.route("/")
def index():

    printid = request.args.get("printid")
    if printid:
        return explore()
    return render_template("index.html")

@app.route("/create", methods=["GET", "POST"])
def create():
    if request.method == "POST":
        cb = []
        for i in range(100):
            cb.append(request.form.get("cb" + str(i)))
            print(cb)
            if not cb[i]:
                cb[i] = 0
            else:
                cb[i] = 1

        db.execute("INSERT INTO prints (cb0, cb1, cb2, cb3, cb4, cb5, cb6, cb7, cb8, cb9, \
            cb10, cb11, cb12, cb13, cb14, cb15, cb16, cb17, cb18, cb19, \
            cb20, cb21, cb22, cb23, cb24, cb25, cb26, cb27, cb28, cb29, \
            cb30, cb31, cb32, cb33, cb34, cb35, cb36, cb37, cb38, cb39, \
            cb40, cb41, cb42, cb43, cb44, cb45, cb46, cb47, cb48, cb49, \
            cb50, cb51, cb52, cb53, cb54, cb55, cb56, cb57, cb58, cb59, \
            cb60, cb61, cb62, cb63, cb64, cb65, cb66, cb67, cb68, cb69, \
            cb70, cb71, cb72, cb73, cb74, cb75, cb76, cb77, cb78, cb79, \
            cb80, cb81, cb82, cb83, cb84, cb85, cb86, cb87, cb88, cb89, \
            cb90, cb91, cb92, cb93, cb94, cb95, cb96, cb97, cb98, cb99) \
            VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, \
            ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, \
            ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, \
            ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, \
            ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);",
            cb[0], cb[1], cb[2], cb[3], cb[4], cb[5], cb[6], cb[7], cb[8], cb[9],
            cb[10], cb[11], cb[12], cb[13], cb[14], cb[15], cb[16], cb[17], cb[18], cb[19],
            cb[20], cb[21], cb[22], cb[23], cb[24], cb[25], cb[26], cb[27], cb[28], cb[29],
            cb[30], cb[31], cb[32], cb[33], cb[34], cb[35], cb[36], cb[37], cb[38], cb[39],
            cb[40], cb[41], cb[42], cb[43], cb[44], cb[45], cb[46], cb[47], cb[48], cb[49],
            cb[50], cb[51], cb[52], cb[53], cb[54], cb[55], cb[56], cb[57], cb[58], cb[59],
            cb[60], cb[61], cb[62], cb[63], cb[64], cb[65], cb[66], cb[67], cb[68], cb[69],
            cb[70], cb[71], cb[72], cb[73], cb[74], cb[75], cb[76], cb[77], cb[78], cb[79],
            cb[80], cb[81], cb[82], cb[83], cb[84], cb[85], cb[86], cb[87], cb[88], cb[89],
            cb[90], cb[91], cb[92], cb[93], cb[94], cb[95], cb[96], cb[97], cb[98], cb[99])
        id = maxID()
        return render_template("explore.html", pixel=cb, loc=id)
    else:
        printid = request.args.get("printid")
        if printid:
            pixel = []
            pixeldata = db.execute("SELECT * FROM prints WHERE id = ?", printid)
            for i in range(1, 101):
                pixel.append(pixeldata[0]["cb" + str(i-1)])
            return render_template("explore.html", pixel=pixel, loc=printid)
        cb = []
        for i in range(100):
            cb.append("cb" + str(i))
        return render_template("create.html", cb=cb)

@app.route("/explore", methods=["GET", "POST"])
def explore():
        pixel = []
        id = request.args.get("printid")
        if not id:
            id = maxID()
        pixeldata = db.execute("SELECT * FROM prints WHERE id = ?", id)
        if not pixeldata:
            id = maxID()
            pixeldata = db.execute("SELECT * FROM prints WHERE id = ?", id)
        for i in range(1, 101):
            pixel.append(pixeldata[0]["cb" + str(i-1)])

        return render_template("explore.html", pixel=pixel, loc=id)

def maxID():
    id = db.execute("SELECT MAX(id) FROM prints")
    id = id[0]["MAX(id)"]
    return id