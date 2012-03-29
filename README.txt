A Python interface to Lotus Notes.
(and a better C++ interface to boot!)

You should just need the notes.pyd file in your python interpreter's path.

Have a look at test_notes.py, which uses a bit of the functionality.

from lotus import notes

# Types
notes.Session
notes.Database
notes.Document
notes.View
notes.Datetime
notes.ViewEntry

# You don't need docs, because the only implemented stuff is obvious...
