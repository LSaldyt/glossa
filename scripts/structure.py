import subprocess, shutil, time, sys, os
from pprint import pprint
from .tools import filenames, dirnames

def build_anyof(directory):
    def filelen(name):
        with open(directory + name, 'r') as content:
            return len([line for line in content])
    names = sorted(filenames(directory), key=filelen, reverse=True)
    names = ['link %s' % name for name in names]
    return 'anyOf %s' % ' '.join(names)

def structure_grammar(language):
    directory = 'languages/' + language + '/grammar/'
    coredir = directory + '.__core__/'
    if os.path.exists(coredir):
        shutil.rmtree(coredir)
    os.makedirs(coredir)

    excluded  = ['core']
    corefiles = [name for name in filenames(directory) if name not in excluded] 

    for filename in corefiles:
        shutil.copy(directory + filename, coredir)

    types = ['value', 'statement', 'misc']
    for t in types:
        type_dir = directory + t + 's/'
        type_files = filenames(type_dir)
        corefiles += type_files
        for filename in type_files:
            shutil.copy(type_dir + filename, coredir)
        if t != 'misc':
            if t == 'statement':
                anyof = build_anyof(type_dir) + '\noptional link comment\n0 sep 1'
            elif t == 'value':
                anyof = build_anyof(type_dir) + '\n0'
            with open(directory + t + '.auto', 'w') as anyoffile:
                anyoffile.write(anyof)
            #shutil.copy(directory + t + '.auto', directory + t)
            #shutil.copy(directory + t + '.bak', directory + t)
            #if t == 'statement':
            shutil.copy(directory + t, coredir)
            corefiles.append(t)

    with open(directory + 'core', 'w') as corefile:
        corefile.write('\n'.join(corefiles))

def build_core_file(language, coretype, excluded=['core']):
    directory = 'languages/' + language + '/' + coretype + '/'
    coredir = directory + '.__core__/'
    if os.path.exists(coredir):
        shutil.rmtree(coredir)
    os.makedirs(coredir)

    corefiles = [name for name in filenames(directory) if name not in excluded] 

    with open(directory + 'core', 'w') as corefile:
        corefile.write('\n'.join(corefiles))

def structure():
    excluded = ['symboltables', 'cython', 'cython3']
    languages = dirnames('languages/')
    for language in languages:
        if language not in excluded:
            structure_grammar(language)
            build_core_file(language, 'constructors', excluded=['core', 'file'])
            build_core_file(language, 'transformers')
