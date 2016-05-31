#!/usr/bin/python

import httplib
import os
import subprocess
import time
import unittest

mainDir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
logFile = mainDir + "/test/essexeld.log"

class TestEssexel(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        global process

        if os.path.exists(logFile):
            os.remove(logFile)

        process = subprocess.Popen([mainDir + "/target/essexeld"], stderr=open(logFile, "w"))
        time.sleep(1)

    @classmethod
    def tearDownClass(cls):
        process.terminate()

    def testBadMethod(self):
        connection = httplib.HTTPConnection("127.0.0.1", 8080)
        connection.request("POST", "http://127.0.0.1:8080/index.html")
        response = connection.getresponse()
        self.assertEqual(response.status, httplib.METHOD_NOT_ALLOWED)

    def testBadUrlPrefix(self):
        connection = httplib.HTTPConnection("127.0.0.1", 8080)
        connection.request("GET", "http://127.0.0.1:8080/index.html")
        response = connection.getresponse()
        self.assertEqual(response.status, httplib.BAD_REQUEST, "index.html -> " + str(response.status))

    def testUnblockedUrl(self):
        connection = httplib.HTTPConnection("127.0.0.1", 8080)
        connection.request("GET", "http://127.0.0.1:8080/urlinfo/1/google.com");
        response = connection.getresponse()
        self.assertEqual(response.status, httplib.NOT_FOUND, "urlinfo/1/google.com -> " + str(response.status))

    def testBlockedUrl(self):
        connection = httplib.HTTPConnection("127.0.0.1", 8080)
        connection.request("GET", "http://127.0.0.1:8080/urlinfo/1/Pornhub.com");
        response = connection.getresponse()
        self.assertEqual(response.status, httplib.OK, "urlinfo/1/Pornhub.com -> " + str(response.status))

if __name__ == '__main__':
    unittest.main()
