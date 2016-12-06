#!/usr/bin/env python3
import subprocess, sys, os

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
    subprocess.run(['./compile.py'] + demos[demoname])

if __name__ == '__main__':
    main()
