CC = g++
CXXFLAGS = -g -Wall `pkg-config --cflags gtk+-2.0 unique-1.0`
LDFLAGS = `pkg-config --libs gtk+-2.0 unique-1.0`

CHANGESET = `hg tip | grep -m 1 changeset: | sed "s/^.*:\s\+\(.*\):.*/\1/"`

gsimplecal:	gsimplecal.o

clean:
	rm -f *.o *~ gsimplecal

install:
	cp ./gsimplecal /usr/bin/

remove:
	rm /usr/bin/gsimplecal

tarball:
	tar -czf ~/gsimplecal-${CHANGESET}.tar.gz --directory .. --exclude=".*" gsimplecal/

