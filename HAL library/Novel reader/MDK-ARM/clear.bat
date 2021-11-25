@echo off
del *.uvguix.*
del *.lst
del *.scvd

for /D %%i in (*) DO (
	rd /S /Q %%i
)

pause