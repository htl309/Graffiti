@echo off
setlocal enabledelayedexpansion


set SHADER_DIR=.

for %%F in (".vert" ".frag" ".comp" ".geom" ".tesc" ".tese" ".task" ".mesh") do (
    for /R %SHADER_DIR% %%f in (*%%F) do (
        glslangValidator.exe -V "%%f" -o "%%f.spv"
        if errorlevel 1 (
            echo compile Error: %%f
        )else (
 		echo compile Success
	)
	echo.
    )
)

echo Finish!!!!!!!!
pause
