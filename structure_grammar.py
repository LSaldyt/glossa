#!/usr/bin/env python3
import subprocess, shutil, time, sys, os
from pprint import pprint

def filenames(directory):
    names = []
    for dirpath, dnames, filenames in os.walk(directory):
        for filename in filenames:
            filepath = os.path.join(dirpath, filename)
            names.append(filename)
    return names

"""
def build_any_of(filename, content):
    with open(filename, 'w') as anyof:
        anyof.write('anyOf %s\n0' % ' '.join(content))

def structure_grammar(language):
    types     = ['statement', 'value']
    directory = 'languages/' + language + '/grammar/'
    core = []
    for t in types:
        type_dir  = '%ss/' % t
        names     = [type_dir + name for name in filenames(directory + type_dir)]
        type_list = ['link %s' % name for name in names]
        build_any_of(directory + t, type_list)
        core += names

    with open(directory + 'core', 'w') as corefile:
        corefile.write('\n'.join(core))

if __name__ == '__main__':
    structure_grammar('python3_grammar_update')
"""

def structure_grammar(language):
    directory = 'languages/' + language + '/grammar/'
    excluded = 'core'
    corefiles = [name for name in filenames(directory) if name not in excluded] 
    with open(directory + 'core', 'w') as corefile:
        corefile.write('\n'.join(corefiles))

if __name__ == '__main__':
    structure_grammar('python3')
    structure_grammar('python2')
    structure_grammar('haskell')
