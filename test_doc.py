
from lotus.notes import *

s = Session()

db = Database("maspubplans")

if not db.is_open():
  print "Couldn't open database."
  exit(1)

view = db.get_view("(IndexByKey)")

doc = Document(db, form="PythonTest")
print """
Document Info
-------------
Created document ok.
  - UNID:        %s
  - Form:        %s
""" % ( doc.unid,
        doc.form )

doc.set("index_cat", "python")
doc.set("index_key", "python-123456")
doc.set("py_string", "ok!")
doc.set("py_float", 3.14)
doc.set("py_float", 3.12)
doc.set("py_float", 3.13)
doc.set("py_float", 3.15)
doc.set("py_int", 42)
doc.set("py_datetime", Datetime("01:24:51 20/07/2009"))
doc.set("py_today", Datetime("today"))
doc.set("py_tomorrow", Datetime("tomorrow"))
doc.save()
print "Saved document to database '%s'" % db.title
print """
We've set some values on the document, now check they can be looked up:

index_cat    ->   %s
index_key    ->   %s
py_string    ->   %s
py_float     ->   %s
py_int       ->   %s
py_datetime  ->   %s
py_today     ->   %s
py_tomorrow  ->   %s
""" % ( doc.get("index_cat"),
        doc.get("index_key"),
        doc.get("py_string"),
        doc.get("py_float"),
        doc.get("py_int"),
        doc.get("py_datetime"),
        doc.get("py_today"),
        doc.get("py_tomorrow") ) 

doc.set("Body", "Ping!")
doc.set("Subject", "Test from Python")
doc.set("SendTo", "Darren Garvey")
doc.set("CopyTo", "Darren Garvey")
doc.form = "Memo"
doc.send()
print "Sent document to '%s'" % "Darren Garvey" #doc.get("SendTo")

