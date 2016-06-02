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

Unit tests for the URL lookup library are in C (using tap, part of sxe):
  1. To build all code and run the unit tests, run: `baker -t`
  2. To manually run the unit tests, run: `test\target\test_essexeld_url`

Data
====

Note that the unit tests now use a domain tables **data/domains** and **data/urls**. These tables were
generated from the MESD blacklist's porn domains and urls lists using the utility program
**util/target/essexeld_blacklist**. The generated tables are sorted lists of MD5
checksums. To regenerate the tables, run the following commands:
  1. `util/target/essexeld_blacklist` *domains-file* > `data/domains`
  2. `util/target/essexeld_blacklist -f` `data/domains` *urls-file* > `data/urls`
  
Initial Benchmark
=================

The script **test/benchmark_essexeld.py** throws the 165719 URLs in the MESD blacklist 
porn urls list at the service. Running the benchmark on a single system (127.0.0.1):

| SXE Build  | Total Time | URLs/s |
| ---------- | ---------- | ------ |
| debug      | 18420s     | 9      |
| release    | 35s        | 4735   |

Using the Server
================

The protocol for looking up the blacklist is simple. Issue an HTTP get request with
the path string /urlinfo/1/host.name[:port][/query/string]. If the URL is not found,
the server will respond with status code 404 (Not Found). If found, the server will
respond with 200 (OK), and the body will contain the string "porn".

Note: The server is currently hard coded to return the category "porn". It could
easily be extended to support multiple categories, or even arbitrary response texts.
The more complex the response, the more memory and CPU data cache will be required by the
service.