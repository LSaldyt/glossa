#!/usr/bin/env python3
import subprocess, shutil, sys, os
from compile import cleardir

def main():
    with open('examples/demos') as demofile:
        content = [line for line in demofile]

    demos = dict()
    for line in content:
        terms = line.split() 
        assert len(terms) > 1
        a = terms[0]
        demos[a] = terms[1:]

    if len(sys.argv) > 1:
        demoname = sys.argv[1]
    else:
        demoname = 'python'

    if not os.path.exists('output'):
        os.makedirs('output')
    if not os.path.exists('input'):
        os.makedirs('input')

    subprocess.run(['./compile.py'] + demos[demoname])

    outputdir = 'examples/' + demoname + '_output'

    if os.path.exists(outputdir):
        shutil.rmtree(outputdir)
    shutil.move('output', outputdir)
    shutil.rmtree('input')

if __name__ == '__main__':
    main()
