#!/usr/bin/env python3
import subprocess, shutil, sys, os

def run(commands):
    try:
        subprocess.run(commands, check=True)
    except subprocess.CalledProcessError as e:
        print("The command (%s) failed with output:" % commands)
        print(e.output)
        raise

def cleardir(dirname):
    for filename in os.listdir(dirname):
        filepath = os.path.join(dirname, filename)
        if os.path.isfile(filepath):
            os.unlink(filepath)

# Remove old compiled files
cleardir('output')

# Build the compiler and test it
os.chdir('build')
run(['cmake', '..'])
run(['make'])
os.chdir('..')
run(['./build/progtrantest'])

# Build the list of input files
if len(sys.argv) == 1:
    directory = 'example/'
else:
    directory = sys.argv[1]

inputfiles = []

for dirpath, dnames, filenames in os.walk(directory):
    for filename in filenames:
        filepath    = os.path.join(dirpath, filename)
        filename, _ = os.path.splitext(filename)
        inputfile   = os.path.join('input/', filename)

        inputfiles.append(filename) # Uses filename, since the compiler knows to use input/output directories
        shutil.copyfile(filepath, inputfile)
        run(['./annotate.py', inputfile])

print('Running files: %s' % '\n'.join(inputfiles))
run(['./build/progtran'] + inputfiles)

# Compile output c++ code
os.chdir('output')
subprocess.run('g++ -std=c++14 *.cpp ../std/*.cpp', shell=True)
run(['./a.out'])
os.chdir('..')

# Cleanup
cleardir('input')
subprocess.run('touch input/.placeholder', shell=True)

