#!/bin/bash

epsToPDF () {
	echo $1
    epstopdf $1
}

if [ ! -d output ]
then
   mkdir output
fi
cd src

# Convert eps to pdf if necessary
find . -name "*.eps" -exec epsToPDF('{}') \;

# Compile 
pdflatex -output-directory ../output book.tex
cd ..
if [ ! -d build ]
then
   mkdir build
fi
cp output/book.pdf build/book.pdf
