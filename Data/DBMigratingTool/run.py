import csv
import sqlite3
from sqlite3 import Error
import datetime

database = "AAA.db"
conn = None
cur = None

def toInt(value):
    try:
        if value == '': return 0
    except:
        pass
    value = value.replace("$","").replace(".00","")
    return int(value)

def toDouble(value):
    if value == '' or value == 0: return 0.0
    value = value.replace("$","")
    return float(value)

def csvToList(fileStr):
    list = []
    with open(fileStr, 'r', encoding='utf-8') as f:
        cr = csv.reader(f)
        for x in cr:
            # print(x)
            list.append(x)
        del list[0]
    return list

def toDate(timeStr):
    if len(timeStr) == 0:
        return "01-01-2000"
#    print(">> toDate", timeStr)
#    d = datetime.datetime.strptime("30-7-2020 0:00:00", "%d-%m-%Y 0:00:00")
    d = datetime.datetime.strptime(timeStr, "%d-%m-%Y 0:00:00")
    return datetime.datetime.strftime(d, "%d-%m-%Y")


def mediaHouse():
    mhl = csvToList("MediaHouse.csv")

    for i,x in enumerate(mhl):
        mhl[i][0] = toInt(mhl[i][0])


    cur.execute("""
    CREATE TABLE "mediaHouse" (
        "id"	INTEGER,
	"name"	TEXT NOT NULL,
    "contactPerson" TEXT,
	"phone"	TEXT,
	"email"	TEXT,
	"address"	TEXT,
	"city"	TEXT,
	"state"	TEXT,
	"GST"	TEXT,
	"SC"	TEXT,
        PRIMARY KEY("id" AUTOINCREMENT)
);
    """)
    conn.commit()


    for x in mhl:
        print(f"[>>] {x}")
        cur.execute(f'INSERT INTO "mediaHouse"("id","name","contactPerson","phone","email","address","city","state","GST","SC") VALUES ("{x[0]}","{x[1]}","","{x[2]}","{x[3]}","{x[4]}","{x[5]}","{x[6]}","{x[7]}","{x[8]}");')
    conn.commit()

    



def parties():
    pl = csvToList("Parties.csv")
    
    for i,x in enumerate(pl):
        pl[i][0] = toInt(pl[i][0])


    conn = sqlite3.connect(database)
    cur = conn.cursor()

    cur.execute("""
    CREATE TABLE "clients" (
	"id"	INTEGER,
	"name"	TEXT NOT NULL,
	"contactperson"	TEXT,
	"phone"	TEXT,
	"email"	TEXT,
	"address"	TEXT,
	"city"	TEXT,
	"state"	TEXT,
	"gst"	TEXT,
	"pincode"	INTEGER,
        PRIMARY KEY("id" AUTOINCREMENT)
);""" )
    conn.commit()

    for x in pl:
        print(f"[>>] {x}")
        cur.execute(f'INSERT INTO "clients"("id","name","contactperson","phone","email","address","city","state","gst","pincode") VALUES ("{x[0]}","{x[1]}","","{x[2]}","{x[3]}","{x[4]}","{x[5]}","{x[6]}","{x[7]}","{x[8]}");')

    conn.commit()
    

def jobType():
    jbl = csvToList("JobType.csv")
    for i,x in enumerate(jbl):
        jbl[i][0] = toInt(jbl[i][0])
    
    conn = sqlite3.connect(database)
    cur = conn.cursor()

    cur.execute("""CREATE TABLE "jobType" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	"name"	TEXT UNIQUE
);""")
    conn.commit()

    for x in jbl:
        print(f'[>>] {x}')
        cur.execute(f'INSERT INTO "main"."jobType"("id","name") VALUES ({x[0]},"{x[1]}");')
    conn.commit()
    

def addInvoiceToRO(rono, invno):
    print(f"[addInvoiceToRO](rono={rono}, invno={invno})")
    cur.execute(f"SELECT invno FROM ro WHERE number = {rono};")
    oinv = cur.fetchone()[0]
    # print(oinv, len(oinv))
    oldinvs = ""
    if oinv != '':
        # print(f"> {oinv}")
        if oinv != "":
            oldinvs = oinv   #"123,454"
            oldinvs += f",{invno}";
    else:
        oldinvs = str(invno)
    # print(f"oldinvs={oldinvs}")
    cur.execute(f'UPDATE ro SET invno = "{oldinvs}" WHERE number = {rono}')
    conn.commit()
        
def invoice():
    inv = csvToList("billg-Invoice.csv")
    for i,x in enumerate(inv):
        inv[i][0] = toInt(inv[i][0])
        inv[i][2] = toInt(inv[i][2])
        inv[i][4] = toDate(inv[i][4])
        inv[i][5] = toInt(inv[i][5])
        inv[i][6] = toInt(inv[i][6])
        inv[i][7] = toInt(inv[i][7])
        inv[i][8] = toDouble(inv[i][8])
        inv[i][11] = toDouble(inv[i][11])
        inv[i][12] = toDouble(inv[i][12])
        inv[i][13] = toDouble(inv[i][13])
        inv[i][14] = toDouble(inv[i][14])
        inv[i][15] = toDouble(inv[i][15])
        inv[i][16] = toDouble(inv[i][16])
        inv[i][17] = toInt(inv[i][17])
        inv[i][20] = toDouble(inv[i][20])
        inv[i][21] = toDouble(inv[i][21])
        inv[i][22] = toDouble(inv[i][22])
        inv[i][23] = toDouble(inv[i][23])
        inv[i][24] = toDouble(inv[i][24])
        inv[i][25] = toDouble(inv[i][25])
        inv[i][26] = toDouble(inv[i][26])


    cur.execute("""CREATE TABLE "invoice" (
    "id"	INTEGER DEFAULT 0,
    "type"	TEXT DEFAULT "",
    "number"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    "invoiceremark"	TEXT DEFAULT "",
    "date"	TEXT,
    "bookno"	INTEGER DEFAULT 0,
    "rocode"	INTEGER DEFAULT 0,
    "rono"	INTEGER,
    "gramount"	REAL,
    "otherch"	TEXT DEFAULT "",
    "otherchamount"	TEXT DEFAULT "",
    "taxrate"	REAL DEFAULT 0,
    "taxamount"	REAL DEFAULT 0,
    "leftamount"	REAL DEFAULT 0,
    "disrate"	REAL,
    "disamount"	REAL,
    "npamount"	REAL,
    "pcode"	INTEGER,
    "remark"	TEXT,
    "totalsizeduration"	TEXT DEFAULT "",
    "ratecgst"	TEXT,
    "amountcgst"	REAL,
    "ratesgst"	TEXT,
    "amountsgst"	REAL,
    "rateigst"	TEXT,
    "amountigst"	REAL,
    "finalamount"	REAL,
    FOREIGN KEY("rono") REFERENCES "ro"("number")
);""")

    conn.commit()

    for i,x in enumerate(inv):
        print(f"[{i+1}][>>] {x}")
        cur.execute(f"""INSERT INTO "invoice"("id","type","number","invoiceremark","date","bookno","rocode","rono","gramount","otherch","otherchamount","taxrate","taxamount","leftamount","disrate","disamount","npamount","pcode","remark","totalsizeduration","ratecgst","amountcgst","ratesgst","amountsgst","rateigst","amountigst","finalamount") 
        VALUES ({x[0]},"{x[1]}",{x[2]},"{x[3]}","{x[4]}",{x[5]},{x[6]},{x[7]},{x[8]},"{x[9]}","{x[10]}",{x[11]},{x[12]},{x[13]},{x[14]},{x[15]},{x[16]},{x[17]},"{x[18]}","{x[19]}","{x[20]}",{x[21]},"{x[22]}",{x[23]},"{x[24]}",{x[25]},{x[26]});""")
        addInvoiceToRO(x[7], x[2])

    conn.commit()
#    


def payment():
    pl = csvToList("Payment.csv")
    for i,x in enumerate(pl):
        print(pl[i])
        pl[i][0] = toInt(pl[i][0])
        pl[i][1] = toInt(pl[i][1])
        pl[i][2] = toDate(pl[i][2])
        pl[i][3] = toDouble(pl[i][3])

    conn = sqlite3.connect(database)
    cur = conn.cursor()

    cur.execute("""
    CREATE TABLE "media_payment" (
	"id"	INTEGER PRIMARY KEY AUTOINCREMENT,
	"rocode"	INTEGER,
	"rono"	INTEGER NOT NULL,
        "date"	TEXT NOT NULL,
	"amount"	REAL NOT NULL,
	"mode"	TEXT,
	"chequeNo"	TEXT,
	"bankname"	TEXT
);
    """)
    conn.commit()

    for i,x in enumerate(pl):
        print(f"[{i+1}][>>] {x}")
        cur.execute(f"""INSERT INTO "media_payment"("id","rocode","rono","date","amount","mode","chequeNo","bankname") VALUES ({i},{x[0]},{x[1]},"{x[2]}",{x[3]},"{x[4]}","{x[5]}","{x[6]}");""")
    conn.commit()
    

def addReceipt(receiptno, rono):
    print(f"addReceipt(rono={rono}, receiptno={receiptno})")
    cur.execute(f"SELECT recptno FROM ro WHERE number = {rono};")
    rcptno = cur.fetchone()
#    print(rcptno)
    oldrcptno = ""
    if rcptno[0] != '':
        print(f"> {rcptno[0]}")
        if rcptno != "":
            oldrcptno = rcptno[0]   #"123,454"
            oldrcptno += f",{receiptno}";
    else:
        oldrcptno = str(receiptno)
    print(f"oldrcptno={oldrcptno}")
    cur.execute(f'UPDATE ro SET recptno = "{oldrcptno}" WHERE number = {rono}')
    conn.commit()

def receipt():
    rl = csvToList("Receipt.csv")
    for i,x in enumerate(rl):
        rl[i][0] = toInt(rl[i][0])
        rl[i][1] = toInt(rl[i][1])
        rl[i][2] = toInt(rl[i][2])
        rl[i][3] = toInt(rl[i][3])
        rl[i][4] = toDate(rl[i][4])
        rl[i][5] = toDouble(rl[i][5])

    cur.execute("""
    CREATE TABLE "payment_receipt" (
            "id"	INTEGER,
            "number"	INTEGER PRIMARY KEY AUTOINCREMENT,
            "rocode"	INTEGER,
            "rono"	INTEGER NOT NULL,
            "rcptDate"	TEXT,
            "rcptamount"	REAL,
            "paymode"	TEXT,
            "chqno"	TEXT,
            "bankname"	TEXT,
            "remark"	TEXT,
            FOREIGN KEY("rono") REFERENCES "ro"("number")
    );
    """)

    conn.commit()
    
    print("\nRECEIPT\n-----------------------------------------\n",
    """  [>>] ["id","number","rocode","rono","rcptDate","rcptamount","paymode","chqno","bankname","remark"] """)
    laterX = []
    for i,x in enumerate(rl):
        print(f"[{i+1}][>>] {x}")
        try:
            cur.execute(f"""INSERT INTO "payment_receipt" ("id","number","rocode","rono","rcptDate","rcptamount","paymode","chqno","bankname","remark") VALUES ({x[0]},{x[1]},{x[2]},{x[3]},"{x[4]}",{x[5]},"{x[6]}","{x[7]}","{x[8]}","{x[9]}");""")
            addReceipt(x[1], x[3])
        except sqlite3.IntegrityError as e:
            laterX.append(x)
            print("Exception Occured: ", e)
    conn.commit()
    for x in laterX:
        try:
            cur.execute(f"""INSERT INTO "payment_receipt" ("id","rocode","rono","rcptDate","rcptamount","paymode","chqno","bankname","remark") VALUES ({x[0]},{x[2]},{x[3]},"{x[4]}",{x[5]},"{x[6]}","{x[7]}","{x[8]}","{x[9]}");""")
            addReceipt(x[1], x[3])
        except sqlite3.IntegrityError as e:
            print("Exception Occured: ", e)
            cur.execute(f"""INSERT INTO "payment_receipt" ("id","rocode","rono","rcptDate","rcptamount","paymode","chqno","bankname","remark") VALUES ({x[0]},{x[2]},{x[3]},"{x[4]}",{x[5]},"{x[6]}","{x[7]}","{x[8]}","{x[9]}");""")
    conn.commit()
    

def mediaBill():
    mb = csvToList("MediaBill.csv")
    for i,x in enumerate(mb):
        mb[i][1] = toDate(mb[i][1])
        mb[i][2] = toInt(mb[i][2])
        mb[i][3] = toInt(mb[i][3])
        mb[i][4] = toDouble(mb[i][4])

    conn = sqlite3.connect(database)
    cur = conn.cursor()
    conn.execute("""CREATE TABLE "mediaBill" (
	"id"	TEXT ,
	"date"	TEXT,
        "rocode"	INTEGER,
        "rono"	NOT NULL,
	"amount"	REAL
);""")

    conn.commit()
    for i,x in enumerate(mb):
        print(f"[{i+1}][>>] {x}")
        cur.execute(f"""INSERT INTO "main"."mediaBill"("id","date","rocode","rono","amount") VALUES ("{x[0]}","{x[1]}",{x[2]},{x[3]},{x[4]});""")
        conn.commit()
        addMediaBillToRO(x[3],x[4])

def addMediaBillToRO(rono, amount):
    print(f"Adding MediaBill to RO rono={rono}, Amount={amount}")
    cur.execute(f"SELECT mbamount FROM ro WHERE number = {rono}")
    amt = cur.fetchone()
#    total = 0;
    if(amt[0] != ''):
        amount+= amt[0];
    cur.execute(f"UPDATE ro SET mbamount = {amount} WHERE number = {rono};")
    conn.commit()


    

def ro():
    rl = csvToList("RelievingOdr.csv")

    for i,x in enumerate(rl):
        rl[i][0] = toInt(rl[i][0])
        rl[i][1] = toInt(rl[i][1])
        rl[i][2] = toDate(rl[i][2])
        rl[i][3] = toInt(rl[i][3])
        rl[i][5] = toInt(rl[i][5])
        rl[i][7] = toInt(rl[i][7])
        rl[i][15] = toDouble(rl[i][15])
        rl[i][17] = toDouble(rl[i][17])
#        rl[i][23] = toInt(rl[i][23])
        rl[i][36] = toDouble(rl[i][36])
        rl[i][19] = toDouble(rl[i][19])
        rl[i][21] = toDouble(rl[i][21])
        rl[i][23] = toDouble(rl[i][23])
        rl[i][25] = toDouble(rl[i][25])
        rl[i][27] = toDouble(rl[i][27])
        rl[i][28] = toDouble(rl[i][28])
        rl[i][29] = toDouble(rl[i][29])
        rl[i][30] = toDouble(rl[i][30])
        rl[i][31] = toDouble(rl[i][31])
        rl[i][32] = toDouble(rl[i][32])
        rl[i][33] = toDouble(rl[i][33])
        rl[i][34] = toDouble(rl[i][34])
        rl[i][35] = toDouble(rl[i][35])

#    conn = sqlite3.connect(database)
#    cur = conn.cursor()

    cur.execute("""
CREATE TABLE "ro" (
        "code"                  INTEGER,
        "number"                INTEGER NOT NULL PRIMARY KEY UNIQUE,
        "date"                  TEXT,
        "mhcode"                INTEGER,
        "mhname"                TEXT,
        "pcode"                 INTEGER,
        "pname"                 TEXT,
        "jobtypecode"           INTEGER,
        "jobtypename"           TEXT,
        "caption"               TEXT,
        "editCentre"            TEXT,
        "doPubtel"              TEXT,
        "sizeduration"          TEXT,
	"totalsizeduration"	TEXT,
        "guarantedpos"          TEXT,
        "premium"               REAL,
        "strPre"                TEXT,
        "rate"                  REAL,
        "strRate"               TEXT,
        "amount"                REAL,
        "netAmount"             REAL,
        "remarks"               TEXT,
        "billAmount"            REAL,
        "invno"                 TEXT,
        "payamount"             REAL,
        "recptno"               TEXT,
        "recptamount"           REAL,
        "mbamount"              REAL,
        "ratecgst"              TEXT,
        "amountcgst"            REAL,
        "ratesgst"              TEXT,
        "amountsgst"            REAL,
        "rateigst"              TEXT,
        "amountigst"            REAL,
        "finalamount"           REAL,
        "hsncode"               INTEGER,
        "disPerc"               REAL DEFAULT 0.0,
    FOREIGN KEY("jobtypecode") REFERENCES "jobtypecode"("id"),
    FOREIGN KEY("mhcode") REFERENCES "mediaHouse"("id")
);
    """)
#    FOREIGN KEY("invno") REFERENCES "invoice"("number"),
    conn.commit()

#    FOREIGN KEY("recptno") REFERENCES "payment_receipt"("number")
    for i,x in enumerate(rl):
        print(f"[{i+1}][>>] {x}")
        cur.execute(f"""INSERT INTO "ro" ("code","number","date","mhcode","mhname","pcode","pname","jobtypecode","jobtypename","caption","editCentre","doPubtel","sizeduration","totalsizeduration","guarantedpos","premium","strPre","rate","strRate","amount","netAmount","remarks","billAmount","invno","payamount","recptno","recptamount","mbamount","ratecgst","amountcgst","ratesgst","amountsgst","rateigst","amountigst","finalamount","hsncode") VALUES ({x[0]},{x[1]},"{x[2]}",{x[3]},"{x[4]}",{x[5]},"{x[6]}","{x[7]}","{x[8]}","{x[9]}","{x[10]}","{x[11]}","{x[12]}","{x[13]}","{x[14]}",{x[15]},"{x[16]}",{x[17]},"{x[18]}",{x[19]},{x[21]},"{x[22]}",{x[23]},"",{x[25]},"",{x[27]},0,"{x[29]}",{x[30]},"{x[31]}",{x[32]},"{x[33]}",{x[34]},{x[35]},{x[36]});""")
    conn.commit()                                                                                                                                                                                                                                                                                                                                                                                                                                                #({x[0]},{x[1]},"{x[2]}",{x[3]},"{x[4]}",{x[5]},"{x[6]}","{x[7]}","{x[8]}","{x[9]}","{x[10]}","{x[11]}","{x[12]}","{x[13]}","{x[14]}",{x[15]},{x[16]},{x[17]},"{x[18]}",{x[19]},{x[20]},"{x[21]}",{x[22]},{x[23]},{x[24]},{x[25]},{x[26]},{x[27]},{x[28]},{x[29]},{x[30]},{x[31]},{x[32]},{x[33]},{x[34]},{x[35]})
#    

import threading

def main():
    d = datetime.datetime.now()
    global conn
    global cur
    conn = sqlite3.connect(database)
    cur = conn.cursor()
    ro()
    receipt()
    invoice()
    jobType()
    parties()
    payment()
    mediaHouse()
    mediaBill()
    conn.close()
    print(f"Time: {datetime.datetime.now() - d}")

try:
    import os
    if os.path.exists(database):
        print(f"{database} Exist, Removing it")
        os.remove(database)
    main()
           
    print("----->> FINISHED SUCESSFULLY <<-------")
except Exception as e:
    print(f"Error Occured, {e}")

input("PRESS ENDTER TO CONTINUE")
