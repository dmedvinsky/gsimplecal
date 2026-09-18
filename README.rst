============
 Gsimplecal
============

Gsimplecal is a lightweight calendar applet written in C++ using GTK.

It was intentionally made for use with tint2_ panel in the openbox_ environment
to be launched upon clock click, but of course it will work without it. In
fact, binding the gsimplecal to some hotkey in you window manager will probably
make you happy. The thing is that when it is started it first shows up, when
you run it again it closes the running instance. In that way it is very easy to
integrate anywhere. No need to write some wrapper scripts or whatever.

Also, you can configure it to not only show the calendar, but also display
multiple clocks for different world timezones. Read the manual page for the
details (there is info about keyboard controls as well!).

.. image:: https://github.com/dmedvinsky/gsimplecal/raw/gh-pages/g/scrot2.png

Binary packages
---------------

There are several maintained packages that I know of:

1. Arch Linux

  ``sudo pacman -S gsimplecal``

2. Debian Linux

  ``sudo apt-get install gsimplecal``

3. Ubuntu Linux

  ``sudo apt-get install gsimplecal``


Building manually
-----------------

Get the source code via git:

  ``git clone git://github.com/dmedvinsky/gsimplecal.git``

Now build it::

  cd gsimplecal
  ./autogen.sh
  ./configure
  make

If you want to use GTK2 instead of GTK3, use ``./configure --enable-gtk2``.

If everything went well, you now have ``./src/gsimplecal`` binary file. You may put it anywhere you want. Optionally, you can install it the usual way. This way it also puts the man page in the appropriate place::

  sudo make install


Dependencies
------------

Runtime
^^^^^^^

- libc
- libstdc++
- libglib
- libgtk3 / libgtk2

Buildtime
^^^^^^^^^

- autotools (autogen, autoconf, automake)
- pkg-config
- libgtk-3-dev / libgtk-2-dev
- C++ compiler (tested only with g++)

Configuration and usage
-----------------------

See ``man gsimplecal`` for configuration details.

.. _tint2: http://code.google.com/p/tint2/
.. _openbox: http://openbox.org/wiki/Main_Page
