#!/usr/bin/env python3

import os
import sys

if __name__ == "__main__":
    strTcJS = os.path.realpath(sys.argv[1])
    strBoost = os.path.realpath(sys.argv[2])
    
    # emsdk is in path, no need to call emsdk_env
    with open("build-config.cmd", "w") as f:
        pass

    with open("build-config.sh", "w") as f:
        pass

    with open("build-config-example.ninja", "r") as f:
        strContents = f.read()

        with open("build-config.ninja", "w") as fOut:
            fOut.write(strContents.replace("<path-to-boost-includes>", strBoost).replace("<path-to-think-cell-public-library-includes>", strTcJS))
    