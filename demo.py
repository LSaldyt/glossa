#!/usr/bin/env python3
import subprocess, shutil, time, sys, os

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
        result = f(*args, **kwargs)
    end    = time.time()
    total  = (end - begin) / iterations
    return result, total

def build(directory, languageargs):
    inputfiles = []

    for dirpath, dnames, filenames in os.walk(directory):
        for filename in filenames:
            filepath    = os.path.join(dirpath, filename)
            filename, _ = os.path.splitext(filename)
            inputfile   = os.path.join('input/', filename)

            inputfiles.append(filename) # Uses filename, since the compiler knows to use input/output directories
            shutil.copyfile(filepath, inputfile)
            if languageargs[0] == 'python':
                run(['./annotate.py', inputfile])

    print('Running files: %s' % '\n'.join(inputfiles))
    run(['./build/progtran'] + languageargs + inputfiles)

def compare(directory, iterations=1):
    # Compile output c++ code
    os.chdir('output')
    subprocess.run('g++ -std=c++14 *.cpp -O3 ../std/*.cpp', shell=True)
    output_time = benchmark(subprocess.run, iterations, './a.out', shell=True)
    print('Output code time:')
    print(output_time[1])
    os.chdir('..')

    olddir = os.getcwd()
    os.chdir(directory)
    input_time = benchmark(subprocess.run, iterations, 'python3 main', shell=True)
    os.chdir(olddir)
    print('Input code time:')
    print(input_time[1])

    print('Speedup:')
    print(input_time[1] / output_time[1])
    

def main():
    # Build the compiler and test it
    os.chdir('build')
    run(['cmake', '..'])
    run(['make'])
    os.chdir('..')
    run(['./build/progtrantest'])

    # Build list of demonstrations
    with open('examples/demos') as demofile:
        content = [line for line in demofile]

    demos = dict()
    for line in content:
        terms = line.split() 
        assert len(terms) > 1
        a = terms[0]
        demos[a] = ['examples/' + a] + terms[1:]

    # Determine which demo to use
    if len(sys.argv) > 1:
        if sys.argv[1] == '--show': # Display which demos are available if --show flag given
            for line in content:
                print(line, end='')
            sys.exit(0)
        demoname = sys.argv[1]      # Otherwise, use demo provided
    else:
        demoname = 'python'         # If none provided, show python demo by default

    if not os.path.exists('output'):
        os.makedirs('output')
    if not os.path.exists('input'):
        os.makedirs('input')

    # Build/compare input/output languages
    l = demos[demoname]
    directory = l[0]
    languageargs = l[1:]
    build(directory, languageargs)
    compare(directory, iterations=100)

    # Save demo output, then cleanup
    outputdir = 'examples/output/' + demoname + '_output'
    if os.path.exists(outputdir):
        shutil.rmtree(outputdir)
    shutil.copytree('output', outputdir)
    shutil.rmtree('output')
    shutil.rmtree('input')

if __name__ == '__main__':
    main()
