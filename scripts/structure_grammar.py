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

def build_anyof(directory):
    names = filenames(directory)
    names = ['link %s' % name for name in names]
    return 'anyOf %s\n0' % ' '.join(names)

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

    types = ['value', 'statement']
    for t in types:
        type_dir = directory + t + 's/'
        type_files = filenames(type_dir)
        corefiles += type_files
        for filename in type_files:
            shutil.copy(type_dir + filename, coredir)
        with open(directory + t, 'w') as anyoffile:
            anyoffile.write(build_anyof(type_dir))
        shutil.copy(directory + t + '.bak', directory + t)
        #shutil.copy(directory + t, coredir)
        #corefiles.append(t)

    with open(directory + 'core', 'w') as corefile:
        corefile.write('\n'.join(corefiles))

def structure_grammars():
    excluded = ['symboltables', 'cython', 'cython3']
    languages = dirnames('languages/')
    for language in languages:
        if language not in excluded:
            structure_grammar(language)
