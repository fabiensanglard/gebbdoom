#!/usr/bin/env bash

# die - Exit with a (integer) return code and message on stderr
die() { rc=$1 && shift && for message in "$@"; do cat <<< "ERROR - $message" >&2; done; exit $rc; }

# Add inkscape to PATH on Mac OS X (located in Contents/Resources/bin up to 0.92.x, Contents/MacOS from 1.x onwards)
#	adding Contents/Resources/bin before Contents/MacOS, since the FS may not be case-sensitive (default for both HFS+ and APFS)
#	and Contents/MacOS/Inkscape (GUI) exists up to 0.92.x, therefore conflicting with Contents/Resources/bin/inkscape (CLI)
[[ $(uname) == "Darwin" ]] && export PATH=$PATH:/Applications/Inkscape.app/Contents/Resources/bin:/Applications/Inkscape.app/Contents/MacOS

# Check tool availability
[[ -x "$(command -v inkscape)" ]] || die 10 "inkscape command is unavailable" # Inkscape
[[ -x "$(command -v epstopdf)" ]] || die 11 "epstopdf command is unavailable" # MacTeX/TeX Live
[[ -x "$(command -v pdflatex)" ]] || die 12 "pdflatex command is unavailable" # MacTeX/TeX Live

# Check Inkscape version and choose export option accordingly (--export-png up to 0.92.x, --export-file from 1.x onwards)
#	falls back on --export-png if unable to read version
[[ $(inkscape --version | awk -F '[ .]' '{ print $2 }') -gt 0 ]] && exportOption="--export-file" || exportOption="--export-png"

# epsToPDF - Convert EPS to PDF
epsToPDF() {
	# Build path without extension
	filename=$(basename "$1")
	filename=${filename%.*}
	dir=$(dirname "$1")

	extensionless=$dir/$filename

	# -nt = newer than, -ot = older than
	if [ "$extensionless".eps -nt "$extensionless".pdf ]; then
		echo "Convert $extensionless.eps to PDF."
		epstopdf "$extensionless".eps
	fi
	# epstopdf $filename
}

# svgToPNG - Convert SVG to PNG
svgToPNG() {
	# Build path without extension
	filename=$(basename "$1")
	filename=${filename%.*}
	dir=$(dirname "$1")

	extensionless=$dir/$filename
	src="$PWD/screenshots_svg/$filename".svg

	# Low RES assets
	dst="$PWD/screenshots/$filename".png
	# -nt = newer than, -ot = older than
	if [[ "$src" -nt "$dst" ]]; then
		echo "Convert $extensionless.svg to PNG (100dpi)."
		inkscape $exportOption="$dst" --without-gui --export-dpi=100 "$src" > /dev/null 2>&1
	fi

	# High RES assets
	dst="$PWD/screenshots_300dpi/$filename".png
	# -nt = newer than, -ot = older than
	if [[ "$src" -nt "$dst" ]]; then
		echo "Convert $extensionless.svg to PNG (300dpi)."
		inkscape $exportOption="$dst" --without-gui --export-dpi=300 "$src" > /dev/null 2>&1
	fi
}

# Prepare
basedir=$(dirname "$0")

cd "$basedir" || die 20 "Project directory is unaccessible"
mkdir -p build output || die 21 "Project directory is unwritable"
cd src || die 22 "Project source directory is unaccessible"

# Convert EPS to PDF if necessary
find . -name "*.eps" | while read -r file; do epsToPDF "$file"; done

# Convert complex SVG drawings to PNG if necessary
find screenshots_svg -name "*.svg" | while read -r file; do svgToPNG "$file"; done

# Compile
pdflatex -output-directory ../output book.tex
cp ../output/book.pdf ../build/book.pdf
