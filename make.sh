#!/bin/bash
export INKSCAPE=/Applications/Inkscape.app/Contents/Resources/bin/inkscape
# set -x
epsToPDF () {	
    # Build path without extension.
	filename=$(basename "$1")
    extension="${filename##*.}"
    filename="${filename%.*}"
	dir=$(dirname $1)

	extensionless=$dir/$filename

  # -nt = newer than. -ot = older than.
	if [ "$extensionless".eps -nt "$extensionless".pdf ]; then
		echo "Convert $extensionless.eps to PDF."
    epstopdf "$extensionless".eps
  fi
  # epstopdf $filename
  exit 0
}

svgToPNG() {
    # Build path without extension.
  filename=$(basename "$1")
    extension="${filename##*.}"
    filename="${filename%.*}"
  dir=$(dirname $1)

  extensionless=$dir/$filename

  src="`pwd`/screenshots_svg/$filename".svg
  dst="`pwd`/screenshots/$filename".png
  # Low RES assets
  # -nt = newer than. -ot = older than.
  if [ ${src} -nt ${dst} ]; then
    echo "Convert $extensionless.svg to PNG (300dpi)."
    ${INKSCAPE} --export-png=${dst} --without-gui --export-dpi=300 ${src} > /dev/null 2>&1
  fi

  dst="`pwd`/screenshots_300dpi/$filename".png
  # High RES assets
  # -nt = newer than. -ot = older than.
  if [ ${src} -nt ${dst} ]; then
    echo "Convert $extensionless.svg to PNG (600dpi)."
    ${INKSCAPE} --export-png=${dst} --without-gui --export-dpi=600 ${src} > /dev/null 2>&1
  fi

}

if [ ! -d output ]
then
   mkdir output
fi
cd src

#Convert complex svg drawings to PNG.
find screenshots_svg -name "*.svg" | while read file; do svgToPNG "$file"; done

# Convert eps to pdf if necessary
find . -name "*.eps" | while read file; do epsToPDF "$file"; done

# Compile 
pdflatex -output-directory ../output book.tex
cd ..
if [ ! -d build ]
then
   mkdir build
fi
cp output/book.pdf build/book.pdf
