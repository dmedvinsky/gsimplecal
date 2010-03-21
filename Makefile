CC = g++
CXXFLAGS = -g -Wall `pkg-config --cflags gtk+-2.0 unique-1.0`
LDFLAGS = `pkg-config --libs gtk+-2.0 unique-1.0`

PREFIX?=/usr/local
CHANGESET = `hg tip | grep -m 1 changeset: | sed "s/^.*:\s\+\(.*\):.*/\1/"`


gsimplecal:	Boxable.o Calendar.o Config.o MainWindow.o gsimplecal.o

clean:
	rm -f *.o *~ gsimplecal

install:
	cp ./gsimplecal ${PREFIX}/bin/

remove:
	rm ${PREFIX}/bin/gsimplecal

tarball:
	tar -czf ~/gsimplecal-${CHANGESET}.tar.gz --directory .. --exclude=".*" gsimplecal/

