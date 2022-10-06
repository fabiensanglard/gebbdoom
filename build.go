package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"strings"
)
import "os"
import "os/exec"

var inkscapeBin = "inkscape"
var epsToPdfBin = "epstopdf"
var mode = "debug"

func run(c string) {
	println(c)
	args := strings.Split(c, " ")
	cmd := exec.Command(args[0], args[1:]...)

	var out bytes.Buffer
	cmd.Stderr = &out

	err := cmd.Run()
	if err != nil {
		println(fmt.Sprintf("Error running cmd '%s'", c))
		println(out.String())
		os.Exit(1)
	}

}

func isOlder(src string, than string) bool {
	stat, err := os.Stat(src)

	// If the file does not exist, it is not older.
	if os.IsNotExist(err) {
		return false
	}

	statThan, _ := os.Stat(than)

	return stat.ModTime().After(statThan.ModTime()) 
}

func checkExecutable(bin string) {
	path, err := exec.LookPath(bin)
	if err != nil {
		fmt.Println(fmt.Sprintf("Could not find executable '%s'", bin))
		os.Exit(1)
	}
	fmt.Println(fmt.Sprintf("Found '%s' -> '%s'", bin, path))
}

func cwd() string {
	cwd, _ := os.Getwd()
	cwd += "/"
	return cwd
}

func convertEPStoPDF(folder string) {
	items, _ := ioutil.ReadDir(folder)
	for _, item := range items {
		if item.IsDir() {
			convertEPStoPDF(folder + item.Name() + "/")
		} else {
			if !strings.HasSuffix(item.Name(), ".eps") {
				continue
			}

			src := folder + item.Name()
			dst := folder + item.Name()
			dst = strings.ReplaceAll(dst, ".eps", ".pdf")

			if isOlder(dst, src) {
				continue
			}
			cmd := epsToPdfBin + " " + src
			run(cmd)
		}
	}
}

func convertSVGtoPNG(folder string) {
	items, _ := ioutil.ReadDir(folder)
	for _, item := range items {
		if item.IsDir() {
			println("Directories in ", folder, " are not supported")
			os.Exit(1)
		} else {

			if !strings.HasSuffix(item.Name(), ".svg") {
				continue
			}

			src := folder + item.Name()
			dirs := []string{cwd() + "src/screenshots/", cwd() + "src/screenshots_300dpi/"}
			dpis := []string{"100", "300"}
			for i, dir := range dirs {
				dst := dir + item.Name()
				dst = strings.ReplaceAll(dst, ".svg", ".png")
				if isOlder(dst, src) {
					continue
				}
				cmd := inkscapeBin + ` --export-filename=` + dst + ` --export-dpi=` + dpis[i] + ` ` + src
				run(cmd)
			}
		}
	}
}

func main() {
	fmt.Println("Building...")

	checkExecutable(inkscapeBin)
	checkExecutable(epsToPdfBin)

	var args = os.Args

	if len(args) > 1 {
		mode = args[1]
	}

	if mode != "debug" && mode != "release" && mode != "print" {
		fmt.Println("Mode must be either 'debug' or 'release' or 'print'.")
		return
	}

	// Convert SVG to PNG
	convertSVGtoPNG(cwd() + "src/screenshots_svg/")
	convertEPStoPDF(cwd())

	// Convert EPS to PDF

	compileOptions := `\def\for` + mode + `{}`

	outputDirName := "out"
	os.MkdirAll(outputDirName, os.ModePerm)

	bin := "pdflatex"
	arg0 := "-output-directory"
	arg1 := outputDirName
	arg2 := compileOptions + ` \input{src/book.tex}`
	fmt.Println(bin, arg0, arg1, arg2)

	out, err := exec.Command(bin, arg0, arg1, arg2).CombinedOutput()

	if err != nil {
		fmt.Println("%s %s", string(out), err)
		return
	}
}
