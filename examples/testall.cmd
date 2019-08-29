@echo off
for /D %%i in (*) DO (
    echo %%i
    cd %%i
    call build || exit 1
    if exist test.cmd (
        call test || exit 1
    )
    cd ..
)
