import csv
import sqlite3
from sqlite3 import Error

database = "AAA.db"

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
    with open(fileStr, 'r') as f:
        cr = csv.reader(f)
        for x in cr:
            list.append(x)
        del list[0]
    return list

def mediaHouse():
    mhl = csvToList("MediaHouse.csv")

    for i,x in enumerate(mhl):
        mhl[i][0] = toInt(mhl[i][0])

    conn = sqlite3.connect(database)
    cur = conn.cursor()

    cur.execute("""
    CREATE TABLE "mediaHouse" (
	"id"	INT,
	"name"	TEXT NOT NULL,
    "contactPerson" TEXT,
	"phone"	TEXT,
	"email"	TEXT,
	"address"	TEXT,
	"city"	TEXT,
	"state"	TEXT,
	"GST"	TEXT,
	"SC"	TEXT,
	PRIMARY KEY("id")
);
    """)
    conn.commit()


    for x in mhl:
        print(f"[>>] {x}")
        cur.execute(f'INSERT INTO "mediaHouse"("id","name","contactPerson","phone","email","address","city","state","GST","SC") VALUES ("{x[0]}","{x[1]}","","{x[2]}","{x[3]}","{x[4]}","{x[5]}","{x[6]}","{x[7]}","{x[8]}");')
    conn.commit()

    conn.close()



def parties():
    pl = csvToList("Parties.csv")
    
    for i,x in enumerate(pl):
        pl[i][0] = toInt(pl[i][0])


    conn = sqlite3.connect(database)
    cur = conn.cursor()

    cur.execute("""CREATE TABLE "clients" (
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
	PRIMARY KEY("id")
);""" )
    conn.commit()

    for x in pl:
        print(f"[>>] {x}")
        cur.execute(f'INSERT INTO "clients"("id","name","contactperson","phone","email","address","city","state","gst","pincode") VALUES ("{x[0]}","{x[1]}","","{x[2]}","{x[3]}","{x[4]}","{x[5]}","{x[6]}","{x[7]}","{x[8]}");')

    conn.commit()
    conn.close()

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
    conn.close()
        
def invoice():
    inv = csvToList("billg-Invoice.csv")
    for i,x in enumerate(inv):
        inv[i][0] = toInt(inv[i][0])
        inv[i][2] = toInt(inv[i][2])
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


    conn = sqlite3.connect(database)
    cur = conn.cursor()

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
    "ratecgst"	REAL,
    "amountcgst"	REAL,
    "ratesgst"	REAL,
    "amountsgst"	REAL,
    "rateigst"	REAL,
    "amountigst"	REAL,
    "finalamount"	REAL
);""")

    conn.commit()

    for i,x in enumerate(inv):
        print(f"[{i+1}][>>] {x}")
        cur.execute(f"""INSERT INTO "invoice"("id","type","number","invoiceremark","date","bookno","rocode","rono","gramount","otherch","otherchamount","taxrate","taxamount","leftamount","disrate","disamount","npamount","pcode","remark","totalsizeduration","ratecgst","amountcgst","ratesgst","amountsgst","rateigst","amountigst","finalamount") 
        VALUES ({x[0]},"{x[1]}",{x[2]},"{x[3]}","{x[4]}",{x[5]},{x[6]},{x[7]},{x[8]},"{x[9]}","{x[10]}",{x[11]},{x[12]},{x[13]},{x[14]},{x[15]},{x[16]},{x[17]},"{x[18]}","{x[19]}",{x[20]},{x[21]},{x[22]},{x[23]},{x[24]},{x[25]},{x[26]});""")

    conn.commit()
    conn.close()

def payment():
    pl = csvToList("Payment.csv")
    for i,x in enumerate(pl):
        pl[i][0] = toInt(pl[i][0])
        pl[i][1] = toInt(pl[i][1])
        pl[i][3] = toDouble(pl[i][3])

    conn = sqlite3.connect(database)
    cur = conn.cursor()

    cur.execute("""
    CREATE TABLE "payment" (
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
        cur.execute(f"""INSERT INTO "payment"("id","rocode","rono","date","amount","mode","chequeNo","bankname") VALUES ({i},{x[0]},{x[1]},"{x[2]}",{x[3]},"{x[4]}","{x[5]}","{x[6]}");""")
    conn.commit()
    conn.close()

def receipt():
    rl = csvToList("Receipt.csv")
    for i,x in enumerate(rl):
        rl[i][0] = toInt(rl[i][0])
        rl[i][1] = toInt(rl[i][1])
        rl[i][2] = toInt(rl[i][2])
        rl[i][3] = toInt(rl[i][3])
        rl[i][5] = toDouble(rl[i][5])

    conn = sqlite3.connect(database)
    cur = conn.cursor()

    cur.execute("""
    CREATE TABLE "receipt" (
	"id"	INTEGER NOT NULL,
	"number"	INTEGER,
	"rocode"	INTEGER,
	"rono"	INTEGER NOT NULL,
	"rcptDate"	TEXT,
	"rcptamount"	REAL,
	"paymode"	TEXT,
	"chqno"	TEXT,
	"bankname"	TEXT,
	"remark"	TEXT
);
    """)
    conn.commit()
    
    for i,x in enumerate(rl):
        print(f"[{i+1}][>>] {x}")
        cur.execute(f"""INSERT INTO "receipt" ("id","number","rocode","rono","rcptDate","rcptamount","paymode","chqno","bankname","remark") VALUES ({x[0]},{x[1]},{x[2]},{x[3]},"{x[4]}",{x[5]},"{x[6]}","{x[7]}","{x[8]}","{x[9]}");""")
    conn.commit()
    conn.close()

def mediaBill():
    mb = csvToList("MediaBill.csv")
    for i,x in enumerate(mb):
        mb[i][2] = toInt(mb[i][2])
        mb[i][3] = toInt(mb[i][3])
        mb[i][4] = toDouble(mb[i][4])

    conn = sqlite3.connect(database)
    cur = conn.cursor()
    conn.execute("""CREATE TABLE "mediaBill" (
	"id"	TEXT ,
	"date"	TEXT,
	"rocode"	INTEGER NOT NULL,
	"rono"	INTEGER,
	"amount"	REAL
);""")

    conn.commit()
    for i,x in enumerate(mb):
        print(f"[{i+1}][>>] {x}")
        cur.execute(f"""INSERT INTO "main"."mediaBill"("id","date","rocode","rono","amount") VALUES ("{x[0]}","{x[1]}",{x[2]},{x[3]},{x[4]});""")
    conn.commit()
    conn.close()

def ro():
    rl = csvToList("RelievingOdr.csv")

    for i,x in enumerate(rl):
        rl[i][0] = toInt(rl[i][0])
        rl[i][1] = toInt(rl[i][1])
        rl[i][3] = toInt(rl[i][3])
        rl[i][5] = toInt(rl[i][5])
        rl[i][7] = toInt(rl[i][7])
        rl[i][15] = toDouble(rl[i][15])
        rl[i][17] = toDouble(rl[i][17])
        rl[i][23] = toInt(rl[i][23])
        rl[i][35] = toInt(rl[i][35])
        rl[i][19] = toDouble(rl[i][19])
        rl[i][20] = toDouble(rl[i][20])
        rl[i][22] = toDouble(rl[i][22])
        rl[i][24] = toDouble(rl[i][24])
        rl[i][26] = toDouble(rl[i][26])
        rl[i][27] = toDouble(rl[i][27])
        rl[i][28] = toDouble(rl[i][28])
        rl[i][29] = toDouble(rl[i][29])
        rl[i][30] = toDouble(rl[i][30])
        rl[i][31] = toDouble(rl[i][31])
        rl[i][32] = toDouble(rl[i][32])
        rl[i][33] = toDouble(rl[i][33])
        rl[i][34] = toDouble(rl[i][34])

    conn = sqlite3.connect(database)
    cur = conn.cursor()

    cur.execute("""
CREATE TABLE "ro" (
	"code"	INTEGER NOT NULL,
	"number"	INTEGER NOT NULL PRIMARY KEY UNIQUE,
	"date"	TEXT,
	"mhcode"	INTEGER,
	"mhname"	TEXT,
	"pcode"	INTEGER,
	"pname"	TEXT,
	"jobtypecode"	INTEGER,
	"jobtypename"	TEXT,
	"caption"	TEXT,
	"editCentre"	TEXT,
	"doPubtel"	TEXT,
	"sizeduration"	TEXT,
	"totalsizeduration"	TEXT,
	"guarantedpos"	TEXT,
	"premium"	REAL,
	"strPre"	TEXT,
	"rate"	REAL,
	"strRate"	TEXT,
	"amount"	REAL,
	"netAmount"	REAL,
	"remarks"	TEXT,
	"billAmount"	REAL,
	"invno"	INTEGER,
	"payamount"	REAL,
	"recptno"	INTEGER,
	"recptamount"	REAL,
	"mbamount"	REAL,
	"ratecgst"	REAL,
	"amountcgst"	REAL,
	"ratesgst"	REAL,
	"amountsgst"	REAL,
	"rateigst"	REAL,
	"amountigst"	REAL,
	"finalamount"	REAL,
	"hsncode"	INTEGER
);
    """)
    conn.commit()

    for i,x in enumerate(rl):
        print(f"[{i+1}][>>] {x}")
        cur.execute(f"""INSERT INTO "ro" ("code","number","date","mhcode","mhname","pcode","pname","jobtypecode","jobtypename","caption","editCentre","doPubtel","sizeduration","totalsizeduration","guarantedpos","premium","strPre","rate","strRate","amount","netAmount","remarks","billAmount","invno","payamount","recptno","recptamount","mbamount","ratecgst","amountcgst","ratesgst","amountsgst","rateigst","amountigst","finalamount","hsncode") VALUES ({x[0]},{x[1]},"{x[2]}",{x[3]},"{x[4]}",{x[5]},"{x[6]}","{x[7]}","{x[8]}","{x[9]}","{x[10]}","{x[11]}","{x[12]}","{x[13]}","{x[14]}",{x[15]},"{x[16]}",{x[17]},"{x[18]}",{x[19]},{x[20]},"{x[21]}",{x[22]},{x[23]},{x[24]},"{x[25]}",{x[26]},{x[27]},{x[28]},{x[29]},{x[30]},{x[31]},{x[32]},{x[33]},{x[34]},{x[35]});""")
    conn.commit()                                                                                                                                                                                                                                                                                                                                                                                                                                                #({x[0]},{x[1]},"{x[2]}",{x[3]},"{x[4]}",{x[5]},"{x[6]}","{x[7]}","{x[8]}","{x[9]}","{x[10]}","{x[11]}","{x[12]}","{x[13]}","{x[14]}",{x[15]},{x[16]},{x[17]},"{x[18]}",{x[19]},{x[20]},"{x[21]}",{x[22]},{x[23]},{x[24]},{x[25]},{x[26]},{x[27]},{x[28]},{x[29]},{x[30]},{x[31]},{x[32]},{x[33]},{x[34]},{x[35]})
    conn.close()

mediaBill()
mediaHouse()
invoice()
jobType()
parties()
payment()
receipt()
ro()
