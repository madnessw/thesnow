@echo off
\masm32\bin\rc /v rsrc.rc
\masm32\bin\cvtres /machine:ix86 rsrc.res
call \masm32\bin\blddll AdvTor
move /y AdvTor.dll ..