#!/usr/bin/env python3
import subprocess, os

subprocess.run(['doxygen', 'Doxyfile'])
os.chdir('docs/doxygen/html')
subprocess.run(['firefox', 'index.html'])
os.chdir('../../../')
