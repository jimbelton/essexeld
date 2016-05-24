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
        self.assertEqual(response.status, httplib.BAD_REQUEST)

if __name__ == '__main__':
    unittest.main()
