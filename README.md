Game Engine Black Book: Doom

This is the source code for the Game Engine Black Book: Doom.
I am releasing the source code (.tex) under GPL license. I
retain ownership of all drawings and only provide them so the
overall thing is compilable.

To compile:
 - Install mactex library (for pdflatex)
 - Install Inkscape
 - run ./make.sh

Note this has only been tested on MacOS X. You will need to adjust the
build script a little bit and probably fix case-sensitive path on Linux
but I believe it should work with minimal work.

To speed up compilation:
 - Use 100 DPI commands in src/mystyle.sty
 - Comment out the part you are not working on in src/book.tex
  
The PDF version looks nicer when cover.tex is included in src/book.tex.

- Fabien Sanglard
