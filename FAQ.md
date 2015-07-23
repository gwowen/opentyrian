### Q:  What is OpenTyrian? ###

**A:**  OpenTyrian is rewrite of [Tyrian](http://en.wikipedia.org/wiki/Tyrian_(video_game))'s Pascal (and x86 assembly) source code in C, replacing the arcane DOS code with SDL.

### Q:  Is Tyrian open source? ###

**A:**  No, only OpenTyrian is open source.  The Tyrian source code was (more or less) licensed to the OpenTyrian developers for an open-source port.

Tyrian was, however, [released as freeware](http://www.freewebs.com/worldtreegames/) (under no specific license) by Jason Emery in August 2004.

### Q:  Why do I get a message along the lines of "`OpenTyrian needs the Tyrian data files to run.`"? ###

**A:**  You need to have the Tyrian 2.1 data files to run the game (1.x, 2.0, or 2000 data files will **NOT** work.)  If you have them (can be obtained from any installed copy of the game) just create a `data` directory and place all the files there.  If you don't, [get yourself a copy](http://sites.google.com/a/camanis.net/opentyrian/tyrian/tyrian21.zip) ([mirror](http://camanis.net/tyrian/tyrian21.zip)).

On Linux, the data files may also be placed in `/usr/share/opentyrian/data/`.

### Q:  I have all the data files inside the ./data/ directory.  Why does OpenTyrian still not find them? ###

**A:**  If you're using a case-sensitive filesystem (most Unix-related filesystems), make sure the filenames are all lower-case. You can use the provided `lower-script.sh` script to rename the files.

### Q:  Why isn't the mouse working as a controller? ###

**A:**  Click the game window, and it will grab your mouse.  Press `<CTRL><F10>` to release it again.

### Q:  Why is a chunk missing from the top of the screen in full-screen mode? ###

**A:**  You probably have an Nvidia graphics card, right?  We think it's a bug in their drivers.  Using one of the scalers (set in the OpenTyrian Options menu) usually avoids it.

### Q:  Why do I get an error about SDL being missing? ###

**A:**  You need the [SDL runtime](http://www.libsdl.org/download.php).  On Windows, just put SDL.dll in OpenTyrian's directory.

### Q:  How can I contact the developers? ###

**A:**  Pretty much all development discussion happens on #tyrian at [freenode](http://freenode.net/). If you have any offers for help or suggestions to the port, the preferred way to contact us is via IRC or filling a new issue report on this site.