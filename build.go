package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"strings"
)
import "os"
import "os/exec"
import "path"

var inkscapeBin = "inkscape"
var epsToPdfBin = "epstopdf"
var mode = "release"
var outputDirName = "out"

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

func toString(fs []string) string {
	var b bytes.Buffer
	for _, s := range fs {
		fmt.Fprintf(&b, "%s ", s)
	}
	return b.String()
}

func makeCover(src string, dst string) {
	src = cwd() + src
	dst = cwd() + dst

	if isOlder(dst, src) {
		return
	}

	bin := inkscapeBin
	args := make([]string, 0)
	args = append(args, "--export-filename="+dst)
	args = append(args, "--export-dpi=300")
	args = append(args, "--export-type=pdf")
	args = append(args, "--export-text-to-path")
	args = append(args, src)

	fmt.Printf("%s %s\n", inkscapeBin, toString(args))

	output, err := exec.Command(bin, args...).CombinedOutput()
	if err != nil {
		fmt.Println("%s %s", string(output), err)
		return
	}
}

func currentDir() string {
	cwd, err := os.Getwd()
	if err != nil {
		panic(err.Error())
	}
	return path.Base(cwd)
}

func getMode() string {
	var args = os.Args
	if len(args) > 1 {
		return args[1]
	}
	return mode
}

func main() {
	mode = getMode()
	fmt.Println("Building in", mode, "mode...")

	checkExecutable(inkscapeBin)
	checkExecutable(epsToPdfBin)

	if mode != "debug" && mode != "release" && mode != "print" {
		fmt.Println("Mode must be either 'debug' or 'release' or 'print'.")
		return
	}

	os.MkdirAll(outputDirName, os.ModePerm)

	// Convert SVG to PNG
	convertSVGtoPNG(cwd() + "src/screenshots_svg/")

	// Convert EPS to PDF
	convertEPStoPDF(cwd())

	// Make front and back cover (this is only used in the non-print version)
	os.MkdirAll(outputDirName+"/cover", os.ModePerm)
	if mode != "print" {
		makeCover("cover/master/front.svg", outputDirName+"/cover/front.pdf")
		makeCover("cover/master/back.svg", outputDirName+"/cover/back.pdf")
	}

	compileOptions := `\def\for` + mode + `{}`

	bin := "pdflatex"
	arg0 := "-output-directory"
	arg1 := outputDirName
	arg2 := compileOptions + ` \input{src/book.tex}`
	draftMode := "-draftmode"

	var err error
	var out []byte

	// Compile in draft mode to generate only necessary files for Table of Contents
	if mode != "debug" {
		fmt.Println(bin, draftMode, arg0, arg1, arg2)
		_, err = exec.Command(bin, draftMode, arg0, arg1, arg2).CombinedOutput()
	}

	// Full Compile to generate PDF
	if err == nil {
		fmt.Println(bin, arg0, arg1, arg2)
		out, err = exec.Command(bin, arg0, arg1, arg2).CombinedOutput()
	}

	if err != nil {
		fmt.Println("%s %s", string(out), err)
	}

	// Rename
	var src = outputDirName + "/book.pdf"
	var dst = outputDirName + "/" + currentDir() + "_" + getMode() + ".pdf"
	os.Rename(src, dst)
}
