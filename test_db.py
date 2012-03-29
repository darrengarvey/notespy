
from lotus.notes import *

s = Session()
db = Database("maspubplans")

print
if not db.is_open():
  print "Couldn't open database."
else:
  print "Opened database: %s" % db.title

print """
Database Info
-------------
%s
  - Server:            %s
  - Path:              %s
  - Filename:          %s
  - Size:              %i bytes
  - Percentage Used:   %2.1f%%
  - Replica ID:        %s
  - Last modified:     %s
  - Last FT indexing:  %s
""" % ( db.title,
        db.server,
        db.path,
        db.filename,
        db.size,
        db.used,
        db.replica_id,
        db.last_modified,
        db.last_ft_indexed )

# Get all the documents in the database and iterate through them.
col = db.docs
print """
Got %i documents. Going to print the UNIDs now:
""" % ( col.count )
for i, doc in enumerate(col):
  print "Document", i + 1, "| UNID:", doc.unid

# Search the database.
print """
Searching database for documents with an `index_cat`.
"""
col = db.search("@IsAvailable(index_cat)")
print "Found", col.count, "matching documents."
for i, doc in enumerate(col):
  print "Document", i + 1, "| UNID:", doc.unid, "| index_cat:", doc.get("index_cat")

print """
Going to attempt to load a non-existant database now.
A RuntimeError will be raised when you try this.
"""
db_that_doesnt_exist = Database("non-existant")

print "This line shouldn't be reached as a runtime error should have been thrown."

