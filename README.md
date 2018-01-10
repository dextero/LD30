Ludum Dare 30 entry by dextero.

Compiling & running under Linux
===============================

Download and compile SFML 2.1
-----------------------------

    $ git submodule update --init
    $ cd extlibs/SFML
    $ cmake .
    $ make

Compile the game
----------------

    $ cd build
    $ cmake .
    $ make

Run the game
------------

From the repository root directory, run:

    $ LD_LIBRARY_PATH=extlibs/SFML/lib ./bin/game


