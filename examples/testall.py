#!/usr/bin/env python3

import os
import subprocess

if __name__ == "__main__":
    strScriptPath = os.path.dirname(os.path.abspath(__file__))

    for strSubdir in os.listdir(strScriptPath):
        strDir = os.path.join(strScriptPath, strSubdir)
        if os.path.isdir(strDir):
            os.chdir(strDir)
            subprocess.check_call([os.path.join(strDir, "build.sh" if os.name=="posix" else "build.cmd")])

            strTestScript = os.path.join(strDir, "test.sh" if os.name=="posix" else "test.cmd")
            if os.path.isfile(strTestScript):
                subprocess.check_call([strTestScript])
                
