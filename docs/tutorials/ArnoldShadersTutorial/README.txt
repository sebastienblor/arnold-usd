To process this tutorial follow these steps:

-Download and install LaTex
   -Install minted latex package
   -Install fancyvrb latex package
   -Install xcolor latex package
   -Install ifplatform latex package
   -Install listings latex package
   -Install mptopdf latex package

-Install Pygments python package
   http://pygments.org/download/
   Download, and extract. Execute this command inside the package
      python setup.py install
   
   Create a file: pygmentize.cmd inside a folder that is in the PATH variable
   with this code:
   
      @echo off
      set PYTHONPATH=C:\Python27
      %PYTHONPATH%\python.exe %PYTHONPATH%\Scripts\pygmentize %*


-Add Arnold lexer to Pyments:
   Copy \mtoa\docs\tutorials\ArnoldShadersTutorial\arnold.py to C:\Python27\Lib\site-packages\pygments\lexers\arnold.py
   In folder C:\Python27\Lib\site-packages\pygments\lexers\ execute: _mapping.py
   

-Execute:
   miktex-pdftex.exe -synctex=1 -undump=pdflatex -shell-escape $fullname
