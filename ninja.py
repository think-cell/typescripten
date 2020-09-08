#!/usr/bin/env python3

import argparse
import json
import os
import re
import subprocess
import sys

# source paths in emscripten file are either relative to SRCDIR or begin with a ninja
def TransformSourcePath(strPath):
	if re.match(r"^\${[A-Za-z]+}/", strPath):
		return strPath
	else:
		return os.path.join("${SRCDIR}", strPath)

if __name__ == "__main__":
	argumentparser = argparse.ArgumentParser()
	argumentparser.add_argument(
		"file",
		help="- .emscripten file describing the files to compile"
	)
	argumentparser.add_argument(
		"config",
		choices=["debug", "release"],
		help="- create ninja file for debug or release"
	)
	argumentparser.add_argument(
		"srcdir",
		nargs="?",
		default=".",
		help="- source file root directory"
	)
	argumentparser.add_argument(
		"outdir",
		nargs="?",
		default=".",
		help="- output directory"
	)  
	argumentparser.add_argument(
		"intdir",
		nargs="?",
		default="obj",
		help="- intermediate file directory"
	)
	args = argumentparser.parse_args()

	strScriptDir = os.path.dirname(os.path.abspath(__file__))
	with open(args.file, "r") as f:
		dictNinja = json.load(f)

	# by default, all relative paths are relative to the input file
	os.chdir(os.path.dirname(os.path.abspath(args.file)))
	os.makedirs(args.intdir, exist_ok=True)
	os.makedirs(args.intdir, exist_ok=True)    
	
	# output files have the same name as input .emscripten file
	strTarget = os.path.splitext(os.path.basename(args.file))[0]
	
	# read files that should be passed to emscripten as --pre-js arguments
	strPreJsDependencies = ""
	liststrPreJs = dictNinja.get("prejs", []) + ["${TCJSDIR}/bootstrap/src/js_callback.js"]
	strPreJsDependencies = " | " + " ".join(map(TransformSourcePath, liststrPreJs))

	liststrCpp = dictNinja["cpp"] + ["${TCJSDIR}/bootstrap/src/js_callback.cpp"]

	# calculate which files emscripten will output depending on the linker flags
	strImplicitOutputs = ""
	with open(os.path.join(strScriptDir, "build-config.ninja"), "r") as fBuildConfig:
		liststrOutputFileEndings = ["wasm"]
		for strLine in fBuildConfig.readlines():
			strLineStripped = strLine.strip()
			if strLineStripped.startswith("COMMON_LINK_FLAGS") or strLineStripped.startswith("DEBUG_LINK_FLAGS" if args.config=="debug" else "RELEASE_LINK_FLAGS"):
				if -1!=strLineStripped.find("--source-map-base"):
					liststrOutputFileEndings.append("wasm.map")
				
				if -1!=strLineStripped.find("--emit-symbol-map"):
					liststrOutputFileEndings.append("js.symbols")

		strImplicitOutputs = " | " + " ".join(map(
			lambda strFileEnding: "${OUTDIR}/"+strTarget+"."+strFileEnding, 
			liststrOutputFileEndings
		))  

	strBuildNinja = os.path.join(args.outdir, strTarget + ".ninja")
	with open(strBuildNinja, "w") as fBuildNinja:
		fBuildNinja.write("include " + os.path.relpath(os.path.join(strScriptDir, "build-config.ninja")) + "\n")

		fBuildNinja.write("INTDIR = " + os.path.abspath(args.intdir) + "\n")
		fBuildNinja.write("OUTDIR = " + os.path.abspath(args.outdir) + "\n")
		fBuildNinja.write("SRCDIR = " + os.path.abspath(args.srcdir) + "\n")
		fBuildNinja.write("TCJSDIR = " + strScriptDir + "\n")

		fBuildNinja.write("CFLAGS = ${COMMON_CFLAGS} " 
			+ ("${DEBUG_CFLAGS}" if args.config=="debug" else "${RELEASE_CFLAGS}") + "\n"
		)
		fBuildNinja.write("LINK_FLAGS = ${COMMON_LINK_FLAGS} " 
			+ ("${DEBUG_LINK_FLAGS}" if args.config=="debug" else "${RELEASE_LINK_FLAGS}")
			+ "".join(map(lambda strPreJs: " --pre-js " + TransformSourcePath(strPreJs), liststrPreJs))
			+ "\n"
		)
		fBuildNinja.write("INCLUDES = -isystem${BOOST_PATH} -isystem${THINK_CELL_PUBLIC_PATH} -I${TCJSDIR}/bootstrap/include" + "\n")

		def ExecShellCommand(strCommand):
			if os.name=="posix":
				return strCommand
			else:
				return "cmd /c \"" + strCommand + "\""
		
		fBuildNinja.write("""
builddir = ${INTDIR}

rule CXX_COMPILER
  depfile = $out.d
  deps = gcc
  command = """ +  ExecShellCommand("em++ $DEFINES $INCLUDES $CFLAGS -MD -MT $out -MF $out.d -o $out -c $in") + """
  description = Building CXX object $out

rule CXX_EXECUTABLE_LINKER
  command = """ + ExecShellCommand("em++ $CFLAGS $LINK_FLAGS $in  -o $out") + """
  description = Linking CXX executable $out
  restat = $RESTAT

rule TCJS 
  command = """ + ExecShellCommand("cd ${TCJSDIR}/stage1 && node main.js $in > $out 2>${INTDIR}/tcjs.log") + """

""")

		strTcJsDependencies = ""
		dictTcJs = dictNinja.get("tcjs", None)
		if dictTcJs:
			fBuildNinja.write("\nbuild ${OUTDIR}/" + dictTcJs["output"] + ": TCJS " + " ".join(map(TransformSourcePath, dictTcJs["inputs"])))
			strTcJsDependencies = " | ${OUTDIR}/" + dictTcJs["output"]
	
		# Create paths for intermediate files inside INTDIR directory
		def TransformIntermediatePath(strPath):
			[str, cReplacements] = re.subn(r"^\${[A-Za-z]+}/", "${INTDIR}/", strPath)
			if 0==cReplacements:
				return os.path.join(os.path.join("${INTDIR}", strPath) + ".o")
			else:
				return str + ".o"

		for strCpp in liststrCpp:
			fBuildNinja.write("\nbuild " + TransformIntermediatePath(strCpp) + ": CXX_COMPILER " + TransformSourcePath(strCpp) + strTcJsDependencies)

		strTargetJs = "${OUTDIR}/" + strTarget + ".js"
		fBuildNinja.write("\nbuild " + strTargetJs + strImplicitOutputs + ": CXX_EXECUTABLE_LINKER " + " ".join(map(TransformIntermediatePath, liststrCpp)) + strPreJsDependencies)

		fBuildNinja.write("\nbuild all: phony " + strTargetJs + "\ndefault all\n")

	subprocess.check_call(["ninja", "-f", strBuildNinja])
		