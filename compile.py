#!/usr/bin/env python3
import subprocess, shutil, sys, os

# Build the compiler
os.chdir('build')
subprocess.run(['cmake', '..'])
subprocess.run(['make'])
os.chdir('..')

# Build the list of input files
if len(sys.argv) == 1:
    directory = './'
else:
    directory = sys.argv[1]

inputfiles = []

for dirpath, dnames, filenames in os.walk(directory):
    for filename in filenames:
        filepath    = os.path.join(dirpath, filename)
        filename, _ = os.path.splitext(filename)
        inputfiles.append(filename)

        inputfile   = os.path.join('input/', filename)
        shutil.copyfile(filepath, inputfile)
        #subprocess.run(['./annotate.py', inputfile])


# Clear output directory
for filename in os.listdir('output'):
    filepath = os.path.join('output', filename)
    if os.path.isfile(filepath):
        os.unlink(filepath)

subprocess.run(['./build/progtran'] + inputfiles)
