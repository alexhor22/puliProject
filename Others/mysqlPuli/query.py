
import MySQLdb

# Open database connection
db = MySQLdb.connect("mydbpuli.clsxlmrwgdqn.us-east-2.rds.amazonaws.com","puliroot","puliroot2017","CommunityPuli" )

# prepare a cursor object using cursor() method
cursor = db.cursor()

# Prepare SQL query to INSERT a record into the database.
sql = "Show tables"
try:
   # Execute the SQL command
   print (cursor.execute(sql))
   print "entra a try"

# Fetch all the rows in a list of lists.
   results = cursor.fetchall()
   for row in results:
      fname = row[0]
      
      # Now print fetched result
      print "fname=%s" % \
            (fname)
      
except:
   print "Error: unable to fetch data"


# disconnect from server
db.close()

