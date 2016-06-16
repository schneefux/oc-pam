#this is the -*- makefile -*- for members.
#
# members is the complement of groups: whereas groups shows the groups a
# specified user belongs to, members shows users belonging to a specified
# group.

# Copyright (c) 1997 by Jim Lynch.
# This software comes with NO WARRANTY WHATSOEVER.
#
# This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; version 2 dated June, 1991, or, at your
#    option, any LATER version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program;  if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# On Debian Linux systems, the complete text of the GNU General
# Public License can be found in `/usr/doc/copyright/GPL' (on some
# installations) or /usr/share/common-licenses/GPL (on newer 
# ones).

CPPFLAGS = -g -Wno-deprecated

members: members.o
	g++ -o members members.o

main.o: members.cc

clean:
	rm -f members *.o *~ core

install: members
	cp members $(DESTDIR)/usr/bin/members
	chmod 755 $(DESTDIR)/usr/bin/members
	chown bin.bin $(DESTDIR)/usr/bin/members
