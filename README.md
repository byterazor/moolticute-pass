# moolticute-pass (mpass)

Command-line client for managing passwords in a mooltipass device using the moolticute daemon.

It is inspired by the linux command line password manager *pass*.

## Author
Dominik Meyer <mailto:dmeyer@federationhq.de>

## License

Copyright 2018 by Dominik Meyer Licensed under GPLv3

## Features

* request password from mooltipass device
* print requested password to stdout or X11 clipboard

## Examples

* `mpass get -s github.de -u byterazor`  will get the password for the github service and the user byterazor and print it to stdout
* `mpass get -s github.de -u byterazor -c` the same but copy the password to the X11 clipboard

## Installation

tbd

## Documentation

The Doxygen documentation can be found at <https://byterazor.github.io/moolticute-pass/> .

## Signed Commits
All commits are signed with the GPG key for dmeyer@federationhq.de valid from 2017-07-30

Fingerprint:  8A96 F2F7 FC07 3485 C724  9879 B4C3 12B6 0060 6B64
