@echo off
for /D %%i in (*) DO (
    echo %%i
    pushd %%i
    if exist get-dependencies.cmd (
        call get-dependencies || exit /b 1
    )
    call build || exit /b 1
    if exist test.cmd (
        call test || exit /b 1
    )
    popd
)
