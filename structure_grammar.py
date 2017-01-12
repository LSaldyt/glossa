#!/usr/bin/env python3
import subprocess, shutil, time, sys, os
from pprint import pprint

def dirnames(directory):
    names = []
    for dirpath, dnames, filenames in os.walk(directory):
        names.extend(dnames)
        break
    return names


def filenames(directory):
    names = []
    for dirpath, dnames, filenames in os.walk(directory):
        names.extend(filenames)
        break
    return names

def structure_grammar(language):
    directory = 'languages/' + language + '/grammar/'
    excluded = 'core'
    corefiles = [name for name in filenames(directory) if name not in excluded] 
    with open(directory + 'core', 'w') as corefile:
        corefile.write('\n'.join(corefiles))

if __name__ == '__main__':
    excluded = ['symboltables', 'cython', 'cython3']
    languages = dirnames('languages/')
    for language in languages:
        if language not in excluded:
            structure_grammar(language)
