
from lotus.notes import *

s = Session()
db = Database("maspubplans")

if not db.is_open():
  print "Couldn't open database."
  exit(1)

print """
Database Info
-------------
%s
  - Server:            %s
  - Path:              %s
  - Filename:          %s
  - Size (bytes):      %s
  - Replica ID:        %s
  - Last modified:     %s
  - Last FT indexing:  %s
""" % ( db.title,
        db.server,
        db.path,
        db.filename,
        db.size,
        db.replica_id,
        db.last_modified,
        db.last_ft_indexed )

view = db.get_view("(IndexByKey)")
print """
View Info
---------
%s
  - count:       %s
  - comment:     %s
  - alias:       %s
  - cache_size:  %s
""" % ( view.name, 
        view.count,
        view.comment,
        view.alias,
        view.cache_size )

print """
Looking up document in view using:
result = Document()
key = "python-123456"
count = view.get(result, key)
"""

result = Document()
key = "python-123456"
count = view.get(result, key)

matches = view.find(key)
print matches.document.unid
print matches.entry
print matches.count

print """
Searching for: '%s'
In view:       '%s'
Matches:       %i
First doc:     '%s'
""" % ( key,
        view.name,
        count,
        result.unid )
result.remove()
print "Removed first result document."
view.refresh()
print "Refreshed view successfully."

doc = Document(db, "PythonTest")
print """
Document Info
-------------
Created document ok.
  - UNID:        %s
  - Form:        %s
""" % ( doc.unid,
        doc.form )

# Update some of the properties of the document...
doc.set("index_cat", "python")
doc.set("index_key", "python-123456")
doc.set("py_string", "ok!")
doc.set("py_float", 3.14)
doc.set("py_float", 42)
doc.set("py_datetime1", Datetime("today"))
doc.set("py_datetime2", Datetime("tomorrow"))

# ... save it ...
doc.save()
print "Saved document to database '%s'" % db.title

# ... and then email it to yours truly.
doc.set("Body", "Ping!")
doc.set("Subject", "Test from Python")
doc.set("SendTo", "Darren Garvey")
doc.set("CopyTo", "Darren Garvey")
doc.form = "Memo"
doc.send()
print "Sent document to '%s'" % "Darren Garvey" #doc.get("SendTo")

