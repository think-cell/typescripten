@ECHO off 

SET fileOut=%2
SHIFT /2

SET "strArgs=%1"
SHIFT

:loop
    IF "%1" NEQ "" (
        SET "strArgs=%strArgs% %1"
        SHIFT
        GOTO loop
    ) ELSE (
        GOTO done
    )

:done

@call node %strArgs% > %fileOut%