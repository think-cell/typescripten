@echo off
for /D %%i in (*) DO (
    echo %%i
    cd %%i
    call build || exit /b 1
    if exist test.cmd (
        call test || exit /b 1
    )
    cd ..
)
