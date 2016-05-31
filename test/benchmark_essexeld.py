#!/usr/bin/python

import httplib
import os
import subprocess
import time

mainDir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
logFile = mainDir + "/test/essexeld.log"
urlFile = mainDir + "/test/urls"
urls    = []

if os.path.exists(logFile):
    os.remove(logFile)

with open(urlFile) as urlStream:
    line = urlStream.readline()

    while line:
        urls.append(line.rstrip())
        line = urlStream.readline()

process = subprocess.Popen([mainDir + "/target/essexeld"], stderr=open(logFile, "w"))
time.sleep(1)
connection = httplib.HTTPConnection("127.0.0.1", 8080)

startTime = time.time()

for url in urls:
    connection.request("GET", "http://127.0.0.1:8080/urlinfo/1/" + url)
    response = connection.getresponse()

    if response.status != httplib.OK:
        print "%s not found (%d)" % (url, response.status)

print "Total time: " + str(time.time() - startTime)
process.terminate()
exit(0)
