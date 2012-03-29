
from lotus.notes import *

s = Session()
db = Database("maspubplans")

if not db.is_open():
  print "Couldn't open database."
  exit(1)

send_to = "Darren Garvey/360Systems"

doc = Document(db, "Memo")
doc.set("Body", "Ping!")
doc.set("Subject", "Test from Python")
doc.set("SendTo", send_to)
doc.send()
print "Sent document to '%s'" % send_to

