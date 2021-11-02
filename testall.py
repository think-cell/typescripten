#!/usr/bin/env python3

import os
import subprocess

if __name__ == "__main__":
    strScriptPath = os.path.dirname(os.path.abspath(__file__))

    nTests = 0
    nTestsSucceeded = 0

    def RunTests(strTestDir):
        global nTests
        global nTestsSucceeded
        for strDir in map(lambda s: os.path.join(strTestDir, s), sorted(os.listdir(strTestDir))):
            if os.path.isdir(strDir):
                os.chdir(strDir)

                strNinjaFile = os.path.join(strDir, "main.ninja")
                if os.path.isfile(strNinjaFile):
                    subprocess.check_output(["ninja", "-f", strNinjaFile, "-t", "clean"], stderr=subprocess.STDOUT)

                strBuildFile = os.path.join(strDir, "build.sh" if os.name=="posix" else "build.cmd")
                if os.path.isfile(strBuildFile):
                    print("================= Building test ", os.path.basename(strDir))
                    nTests += 1
                    try:
                        subprocess.check_output([strBuildFile], stderr=subprocess.STDOUT)
                        print("[SUCCESS]\n")

                        strTestScript = os.path.join(strDir, "test.sh" if os.name=="posix" else "test.cmd")
                        if os.path.isfile(strTestScript):
                            print("================= Running test ", os.path.basename(strDir))
                            subprocess.check_call([strTestScript])
                            print("[SUCCESS]\n")

                        nTestsSucceeded += 1
                            
                    except subprocess.CalledProcessError as e:
                        print("[FAILED]\n")
                        if e.stdout:
                            print(e.stdout.decode())

    RunTests(os.path.join(strScriptPath, "bootstrap/tests"))

    try:
        print("================= Building typescripten compiler")
        os.chdir(os.path.join(strScriptPath, "typescripten"))
        subprocess.check_output([os.path.join(strScriptPath, "typescripten", "build.sh" if os.name=="posix" else "build.cmd")], stderr=subprocess.STDOUT)
        print("[SUCCESS]\n")

        RunTests(os.path.join(strScriptPath, "typescripten", "tests"))

        print("{}/{} succeeded".format(nTestsSucceeded, nTests))
        exit(0 if nTests==nTestsSucceeded else 1)

    except subprocess.CalledProcessError as e:
        print("[FAILED]\n")
        if e.stdout:
            print(e.stdout.decode())
        exit(1)

