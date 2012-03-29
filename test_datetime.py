
from lotus.notes import *

s = Session()

print """
Datetime Info
-------------

Datetime("Now")             ==  %s
Datetime("13:50:20 4/99")   ==  %s
""" % ( Datetime("Now"),
        Datetime("13:50:20 4/3/99") )

# Get the current time. Note that 'now' is case-insensitive.
dt = Datetime("nOW")

# Adjust the time and print the resulting datetime.
# str() ensures the value of the datetime is copied after each adjustment. 
print """
Now:                %s
+1 hour:            %s
-10 mins:           %s
+90 secs:           %s
+32 days:           %s
1 month, 1 second:  %s
-100 years:         %s
""" % ( str(dt),
        str(dt.adjust(hours=1)),
        str(dt.adjust(mins=-10)),
        str(dt.adjust(secs=90)),
        str(dt.adjust(days=32)),
        str(dt.adjust(months=1, secs=1)),
        str(dt.adjust_date(-100,0,0)) )

