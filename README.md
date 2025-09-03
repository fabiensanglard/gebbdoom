# Game Engine Black Book: Doom

This is the source code for the Game Engine Black Book: Doom.
I am releasing the source code (.tex) under GPL license. I
retain ownership of all drawings and only provide them so the
overall thing is compilable.

To compile on Mac OS X:
 - Install golang: https://go.dev/
 - Install Inkscape (min  v1.2.2)
    - via installer: https://inkscape.org/download/
    - or via Homebrew: `brew install --cask xquartz inkscape`
 - Install MacTeX library (for epstopdf and pdflatex):
    - via installer: https://tug.org/mactex/mactex-download.html
    - or via Homebrew, with GUI:    `brew install --cask mactex`
    - or via Homebrew, without GUI: `brew install --cask mactex-no-gui`
 - Make sure inkscape binary is in your PATH or symlink it where your PATH points to.  
 - Run:
    - `./make.sh`

To compile on Debian GNU/Linux and derivatives (Ubuntu):
 - Install Inkscape and TeX Live library (for epstopdf and pdflatex):
    - `sudo apt install inkscape texlive texlive-font-utils texlive-fonts-recommended texlive-latex-extra golang-go`
 - Run:
    - `./make.sh`

To compile on Windows:
 - Install WSL1 or WSL2.
 - Follow Linux instructions. 

To speed up compilation:
 - Build with `./make.sh debug` (uses 100 DPI assets)
 - Comment out the part you are not working on in src/book.tex

_Fabien Sanglard_
