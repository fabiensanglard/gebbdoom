# Game Engine Black Book: Doom

This is the source code for the Game Engine Black Book: Doom.
I am releasing the source code (.tex) under GPL license. I
retain ownership of all drawings and only provide them so the
overall thing is compilable.

To compile on Mac OS X:
 - Install Inkscape:
    - via installer: https://inkscape.org/download/
    - or via Homebrew: `brew cask install xquartz inkscape`
 - Install MacTeX library (for epstopdf and pdflatex):
    - via installer: https://tug.org/mactex/mactex-download.html
    - or via Homebrew, with GUI:    `brew cask install mactex`
    - or via Homebrew, without GUI: `brew cask install mactex-no-gui`
 - Run:
    - `./make.sh`

To compile on Debian GNU/Linux and derivatives (Ubuntu):
 - Install Inkscape and TeX Live library (for epstopdf and pdflatex):
    - `sudo apt install inkscape texlive texlive-font-utils texlive-latex-extra`
 - Run:
    - `./make.sh`

To speed up compilation:
 - Use 100 DPI commands in src/mystyle.sty
 - Comment out the part you are not working on in src/book.tex
  
The PDF version looks nicer when cover.tex is included in src/book.tex.

_Fabien Sanglard_
