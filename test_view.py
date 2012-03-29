
from lotus.notes import *

s = Session()

db = Database("maspubplans")

if not db.is_open():
  print "Couldn't open database."
  exit(1)

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
Looking up document in view using something like:

    key = "python-123456"
    match = view.find(key)
    if match.count > 0:
        entry = match.entry
        doc = match.document
"""

key = "python-123456"
match = view.find(key)

# When you look up a view entry, the document it references
# isn't opened by default. For some values, you need to open
# the document before looking them up. From the properties used 
# below, reading last_accessed requires the document is opened.
# 
# A RuntimeError is thrown when you need to do this and don't.
match.document.open()

print """
Searched for:                '%s'
In view:                     '%s'
Matches:                      %i
""" % ( key,
        view.name,
        match.count)

entry = match.entry

print """
ViewEntry info
--------------
Child count:                %i
Doc level:                  %i
Descendent count:           %i
Sibling count:              %i
FTSearch score:             %i

Is main topic:              %s
Is category:                %s
Is response:                %s
Is total:                   %s
""" % ( entry.child_count,
        entry.doc_level,
        entry.descendent_count,
        entry.sibling_count,
        entry.ft_search_score,
        entry.is_main_topic(),
        entry.is_category(),
        entry.is_response(),
        entry.is_total() )

doc = match.document

print """
Document info
-------------
First doc (unid):            '%s'
First doc (created):         '%s'
First doc (last accessed):   '%s'
First doc (last modified):   '%s'
""" % ( doc.unid,
        doc.created,
        doc.last_accessed,
        doc.last_modified )

# You can also search a view by passing a list of strings to find.
# The strings are compared against the columns in the view in
# the order they're passed to this function.
# If more keys are passed in than the view has columns a RuntimeError
# will be thrown.
index = db.get_view("(Index)")
matches = index.find(["PythonTest", "python"])
print """
Searching (Index) view with multiple keys. In this case, this method should
find the same number of documents as above. The code looks like:

    index = db.get_view("(Index)")
    matches = index.find(["PythonTest", "python"])

Matches.count = %s
""" % matches.count

view.refresh()
print "Refreshed view successfully."

for i, entry in enumerate(view.entries):
  doc = entry.document
  doc.open()
  print "UNID:", doc.unid, "| form =", doc.form, "| key =", doc.get("index_key")

#for i, entry in enumerate(view):
#  #entry.document.open()
#  #print i
#  #print i, entry.document.form
#  pass

