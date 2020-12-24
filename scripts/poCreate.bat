set path=%path%;F:\Programs\gettext21\bin
cd ..
set FILES_PATH=%CD%\build\files.txt
set TRANSLATION_PATH=%CD%\build\wxTranslation.po
cd src\frontend\gui
dir /B *.c* > %FILES_PATH%
xgettext --from-code=cp1251 -a --no-location -s --no-wrap -f%FILES_PATH% -o%TRANSLATION_PATH%
pause