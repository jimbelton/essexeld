#!/usr/bin/python

import httplib
import os
import subprocess
import unittest

mainDir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

class TestEssexel(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        global process
        process = subprocess.Popen([mainDir + "/target/essexeld"])

    @classmethod
    def tearDownClass(cls):
        process.terminate()

    def test_connect(self):
        connection = httplib.HTTPConnection("127.0.0.1", 8080)
        connection.request("GET", "http://127.0.0.1:8080/index.html")

if __name__ == '__main__':
    unittest.main()
