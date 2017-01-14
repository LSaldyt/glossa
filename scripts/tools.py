import os

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
