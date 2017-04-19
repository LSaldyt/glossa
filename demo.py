#!/usr/bin/env python3
import subprocess, shutil, time, sys, os
from pprint import pprint
from scripts import annotate, structure, analyze

def run(commands):
    try:
        subprocess.run(commands, check=True)
    except subprocess.CalledProcessError as e:
        print("The command (%s) failed with output:" % commands)
        print(e.output)
        raise

def cleardir(dirname):
    for filename in os.listdir(dirname):
        filepath = os.path.join(dirname, filename)
        if os.path.isfile(filepath):
            os.unlink(filepath)

def benchmark(f, iterations, *args, **kwargs):
    begin  = time.time()
    for i in range(iterations):
        f(*args, **kwargs)
    end    = time.time()
    total  = (end - begin) / iterations
    return total

def build(directory, languageargs, verbosity):
    inputfiles = []

    for dirpath, dnames, filenames in os.walk(directory):
        for filename in filenames:
            filepath    = os.path.join(dirpath, filename)
            filename, _ = os.path.splitext(filename)
            inputfile   = os.path.join('input/', filename)

            inputfiles.append(filename) # Uses filename, since the compiler knows to use input/output directories
            shutil.copyfile(filepath, inputfile)
            if languageargs[0] in ['python', 'python2', 'python3', 'python_core', 'auta', 'a']:
                annotate(inputfile)

    print('Running files: %s' % '\n'.join(inputfiles))
    t = benchmark(run, 1, ['./build/glossa', str(verbosity)] + languageargs + inputfiles)
    print('Compilation took roughly: %ss' % t)

def time_run(language, directory, iterations, filename):
    with open('languages/' + language + '/run', 'r') as runfile:
        content = [line.replace('@', filename) for line in runfile]
    olddir = os.getcwd()
    shutil.copytree(directory, directory + '_copy')
    os.chdir(directory + '_copy')
    try:
        for line in content[:-1]:
            subprocess.call(line, shell=True) # Yes, this is unescaped.
        t = benchmark(subprocess.call, iterations, content[-1], shell=True)
    finally:
        os.chdir(olddir)
        shutil.rmtree(directory + '_copy')
    return t

def run_language(directory, inputlang, outputlang):
    # Compile output c++ code (hardcoded for now, since output language is always c++)
    if outputlang == 'cpp':
        os.chdir('output')
        subprocess.run('g++ -std=c++17 *.cpp -O2 ../std/cpp/*.cpp', shell=True)
        subprocess.run('./a.out', shell=True)
        os.chdir('..')
    else:
        time_run(outputlang, 'output', 1, 'main')

def compare(directory, inputlang, outputlang, iterations=1):
    print('Input code:')
    input_time = time_run(inputlang, directory, iterations, 'main')
    print('%s seconds total' % input_time)

    # Compile output c++ code (hardcoded for now, since output language is always c++)
    print('Output code:')
    if outputlang == 'cpp':
        os.chdir('output')
        subprocess.run('g++ -std=c++17 *.cpp -O2 ../std/cpp/*.cpp', shell=True)
        output_time = benchmark(subprocess.call, iterations, './a.out', shell=True)
        os.chdir('..')
    else:
        output_time = time_run(outputlang, 'output', iterations, 'main')
    print('%s seconds total' % output_time)

    if inputlang in ['python2', 'python3']:
        cythonversion = 'cython3' if inputlang == 'python3' else 'cython'
        print('Cython code:')
        cython_time = time_run(cythonversion, directory, iterations, 'main')
        print('%s seconds total' % cython_time)

def load_demos():
    # Build list of demonstrations
    with open('examples/demos') as demofile:
        content = [line for line in demofile]

    demos = dict()
    for line in content:
        terms = line.split() 
        assert len(terms) > 1
        a = terms[0]
        demos[a] = ['examples/' + a] + terms[1:]
    return demos

def transpile(demoname, demos, verbosity, runcomp=False, runlang=True):
    if not os.path.exists('output'):
        os.makedirs('output')
    if not os.path.exists('input'):
        os.makedirs('input')

    try:
        # Build/compare input/output languages
        l = demos[demoname]
        directory = l[0]
        languageargs = l[1:]
        build(directory, languageargs, verbosity)
        if languageargs[0] in ['python2', 'python3', 'auta', 'fortran']:
            if os.path.exists('output/__pylib__'):
                shutil.rmtree('output/__pylib__')
            shutil.copytree('std/__pylib__/', 'output/__pylib__')
	# Save demo output before trying to run anything else
        outputdir = 'examples/output/' + demoname + '_output'
        if os.path.exists(outputdir):
            shutil.rmtree(outputdir)
        print('Copying output to %s' % outputdir)
        shutil.copytree('output/', outputdir)

        if runcomp:
            compare(directory, languageargs[0], languageargs[1], iterations=1)
        elif runlang:
            run_language(directory, languageargs[0], languageargs[1])
    finally:
        shutil.rmtree('output')
        shutil.rmtree('input')

def main():
    structure()
    # Build the compiler and test it
    os.chdir('build')
    run(['cmake', '..'])
    run(['make'])
    os.chdir('..')
    run(['./build/glossatest'])

    demos = load_demos()

    if len(sys.argv) > 2:
        verbosity = sys.argv[2]
    else:
        verbosity = 1

    # Determine which demo to use
    if len(sys.argv) > 1:
        if sys.argv[1] == '--show': # Display which demos are available if --show flag given
            for demo, info in demos.items():
                print('%-20s: %s -> %s' % (demo, info[1], info[2]))
            sys.exit(0)
        if sys.argv[1] == '--test': # Test each demo
            for demo in demos:
                try:
                    transpile(demo, demos, verbosity, runlang=False) # Test that each demo works
                except Exception as e:
                    print('Transpile test for demo %s failed' % demo)
                    raise e
        if sys.argv[1] == '--analyze':
            analyze()
            sys.exit(0)
        demoname = sys.argv[1]      # Otherwise, use demo provided
    else:
        demoname = 'python3'         # If none provided, show python demo by default


    transpile(demoname, demos, verbosity, runlang=True)
    #transpile(demoname, demos, verbosity, runlang=False)


if __name__ == '__main__':
    main()
