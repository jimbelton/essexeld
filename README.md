essexeld
========

C service for serving information about URLs over HTTP from Linux

Description
===========

**essexeld** is a service that serves information about URLs over HTTP. It was built and tested on Mint/debian Linux.

Dependencies
============

sxe
---

**essexeld** requires the latest version of **libsxe**:

  1. Run: `git clone https://github.com/jimbelton/sxe`
  2. Run: `cd sxe`
  3. Run: `make production`

baker
-----

**essexeld** can be built using my experimental **baker** build tool:
  1. Run:  `git clone https://github.com/jimbelton/baker`
  2. Add the **baker** directory to your path if desired

Building
========

To build, simple run: `baker`

Testing
=======

Tests are written in python. Run: `test/essexeld.py`
