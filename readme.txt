indiprop - INDI Property Browser - v1.03
_____________________________________________________________________

This program provides a generic user interface for browsing device 
properties published by an INDI compliant server.

Project home: [http://code.google.com/p/indiprop/]

INDI is the Instrument Neutral Distributed Interface.  For more 
information see the INDI white-paper at:
[http://www.clearskyinstitute.com/INDI/INDI.pdf]
_____________________________________________________________________

Prerequisites:

Building this program requires a Qt6 development environment.
Qt6 is available for many platforms including Windows, Mac, and Linux. 
_____________________________________________________________________

Building:

From the project folder type:

qmake6
make
_____________________________________________________________________

Installation: 

Installs to $OBSHOME directory with the following:

make install
_____________________________________________________________________

Copyright © 2012 Aaron Evers

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see [http://www.gnu.org/licenses/].
_____________________________________________________________________

Changelog:

v1.03 - Updated to build with Qt6

v1.02 - Added --xml option to dump raw indi xml to stdout

v1.01 - Added message pane

v1.00 - Initial release
