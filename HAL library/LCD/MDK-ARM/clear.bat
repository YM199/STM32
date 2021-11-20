@echo off
del *.uvguix.*
del *.lst

for /D %%i in (*) DO (
	rd /S /Q %%i
)

pause