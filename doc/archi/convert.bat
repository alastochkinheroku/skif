del /s /q /f proc/*.puml
del /s /q /f proc/*.png
del /s /q /f proc/*.cmapx
del /s /q /f proc/*.html
copy *.puml proc
cd proc
java -jar C:\workspace\tools\plantuml.jar -charset UTF-8 *.puml
python png_cmapx_to_html.py
copy *.html "../out" /Y
copy *.png "../out" /Y
python -m markdown2 ../architect.md >> ../out/index.html
pause