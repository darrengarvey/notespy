
from lotus.notes import *

s = Session("C:/Code/C++/0.misc/python/1.hello_class/notes.1.ini")

# Construct a User object using the current user.
user = User()
print """
User Info
---------
Name:
  - Canonical Name:    %s
  - Abbreviated Name:  %s
Mail Server:
  - Canonical Name:    %s
  - Abbreviated Name:  %s
Mail File:             %s
""" % ( user.name, # already in canonical format, could have used user.name.cn
        user.name.abbr,
        user.mail_server, # already in canonical format
        user.mail_server.abbr,
        user.mail_db.title )

