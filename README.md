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

**essexeld** requires the *latest* version of **sxe**:

  1. Run: `git clone https://github.com/jimbelton/sxe`
  2. Run: `cd sxe`
  3. Run: `make production`

baker
-----

**essexeld** can be built using the latest version of my experimental **baker** 
build tool:
  1. Run:  `git clone https://github.com/jimbelton/baker`
  2. Add the **baker** directory to your path if desired

Building
========

To build, simple run: `baker`

Testing
=======

Functional tests are written in python. Run: `test/essexeld.py`

Unit tests for the URL lookup library are in C (using tap, part of sxe)
To build all code and run the unit tests, run: `baker -t`
To manually run the unit tests, run: `test\target\test_essexeld_url`

Data
====

Note that the unit tests now use a domain table **data/domains**. This table was
generated from the MESD blacklist's porn domain list using the utility program
**util/target/essexeld_blacklist**. The generated table is a sorted list of MD5
checksums.