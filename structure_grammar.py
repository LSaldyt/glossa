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
    coredir = directory + '.__core__/'
    if os.path.exists(coredir):
        shutil.rmtree(coredir)
    os.makedirs(coredir)

    excluded  = 'core'
    corefiles = [name for name in filenames(directory) if name not in excluded] 

    for filename in corefiles:
        shutil.copy(directory + filename, coredir)

    types = ['values', 'statements']
    for t in types:
        type_dir = directory + t + '/'
        type_files = filenames(type_dir)
        corefiles += type_files
        for filename in type_files:
            shutil.copy(type_dir + filename, coredir)

    with open(directory + 'core', 'w') as corefile:
        corefile.write('\n'.join(corefiles))


if __name__ == '__main__':
    excluded = ['symboltables', 'cython', 'cython3']
    languages = dirnames('languages/')
    for language in languages:
        if language not in excluded:
            structure_grammar(language)
