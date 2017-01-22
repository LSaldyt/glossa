from __future__ import print_function
from contextlib import contextmanager
import subprocess, os, sys

# Use a directory for a task
@contextmanager
def directory(directoryName):
    oldwd = os.getcwd()
    os.chdir(directoryName)
    try:
        yield
    finally:
        os.chdir(oldwd)

# Aliases
move_to = os.chdir
get_dir = os.getcwd

# Get decoded output of a command, even if it fails!
def output_of(commands, shell=False):
    try:
        # We'll want error output, too
        output = subprocess.check_output(commands, stderr=subprocess.STDOUT)
    except subprocess.CalledProcessError as e:
        print(e)
        output = e.output
    return output.decode('utf-8')

# Immediate subdirectories
def subdirs(directory):
    for _, dirs, _ in os.walk(directory):
        return dirs 
    return [] # If directory is empty

# Immediate files
def subfiles(directory):
    for _, _, files in os.walk(directory):
        return files 
    return []

def files(directory):
    allfiles = []
    for subdir, _, files in os.walk(os.getcwd()):
        for name in files:
            allfiles.append(name)
    return allfiles
            
# Full paths
def filenames(directory):
    filenames = []
    for subdir, _, files in os.walk(os.getcwd()):
        for name in files:
            filenames.append(subdir + os.sep + name)
    return filenames

# 'Parse' git branch output
def git_branchname():
    branches   = []
    output     = get_output(['git', 'branch'])
    branches   = output.splitlines()
    for branch in branches:
        if '*' in branch: # current branch is starred
            return branch.replace('*', '').replace(' ', '') # Get rid of * and whitespace

def git_branches():
    branches   = []
    output     = get_output(['git', 'branch'])
    # Remove whitespace and *
    branches   = output.replace('*', '').replace(' ', '').splitlines()
    return branches

def git_author(SHA=None):
    if SHA is None:
        output = subprocess.check_output(['git', 'log', 'HEAD', '-1'])
    else:
        output = subprocess.check_output(['git', 'show', SHA])
    authorline = output.decode('utf-8').splitlines()[1]
    _, author, email = authorline.split()
    return author, email


def git_changed_files():
    output = get_output(['git', 'diff', '--name-only'])
    return output.splitlines()
