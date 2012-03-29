
from lotus.notes import *

s = Session()
db = Database("maspubplans")

agents = db.agents
print """
Got %i agents. Going to print the names now:
""" % ( agents.count )
for i, agent in enumerate(agents):
  print "Agent", i + 1, \
        "| Enabled:", agent.enabled, \
        "| Name:", agent.name, \
        "| Last Accessed:", agent.last_accessed

