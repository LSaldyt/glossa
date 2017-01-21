#!/usr/bin/env python3
import os
from .tools import filenames, dirnames
from pprint import pprint

def grammar_files(language):
    with open('languages/' + language + '/grammar/core', 'r') as corefile:
        return {item.replace('\n', '') for item in corefile if '.' not in item}

def constructor_files(language):
    with open('languages/' + language + '/constructors/core', 'r') as corefile:
        return {item.replace('\n', '') for item in corefile if '.' not in item}

def analyze():
    excluded = ['symboltables', 'cython', 'cython3']
    languages = [dirname for dirname in dirnames('languages') if dirname not in excluded]
    languageDict = {language : (grammar_files(language), constructor_files(language)) for language in languages}

    for a in languageDict:
        aGrammar      = languageDict[a][0]
        aConstructors = languageDict[a][1]
        for b in languageDict:
            if a != b:
                bGrammar      = languageDict[b][0]
                bConstructors = languageDict[b][1]
                print('Needed constructor files for conversion from %s to %s:' % (a, b))
                pprint(aGrammar - bConstructors)
                #print('Needed constructor files for conversion from %s to %s:' % (b, a))
                #pprint(bGrammar - aConstructors)
