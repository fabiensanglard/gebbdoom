if [ ! -d output ]
then
   mkdir output
fi
cd src
find . -name "*.eps" -exec epstopdf '{}' \;
pdflatex -output-directory ../output book.tex
cd ..
if [ ! -d build ]
then
   mkdir build
fi
cp output/book.pdf build/book.pdf
