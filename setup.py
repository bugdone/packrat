import sys
from cx_Freeze import setup, Executable


setup(name = "packrat",
      version = "0.4",
      description = "packrat",
      options= {'build_exe': {'include_msvcr':True}},
      executables = [Executable("packrat.py")])
