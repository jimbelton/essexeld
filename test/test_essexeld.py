#!/usr/bin/python

import os
import subprocess
import unittest

mainDir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

class TestEssexel(unittest.TestCase):

    def test_spawn(self):
        process = subprocess.Popen([mainDir + "/target/essexeld"])

if __name__ == '__main__':
    unittest.main()
