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
    

def main():
    # Remove old compiled files
    cleardir('output')

    # Build the compiler and test it
    os.chdir('build')
    run(['cmake', '..'])
    run(['make'])
    os.chdir('..')
    run(['./build/progtrantest'])

    languageargs = []

    # Build the list of input files
    assert len(sys.argv) == 4
    directory = sys.argv[1]
    languageargs = sys.argv[2:]

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

    # Compile output c++ code
    os.chdir('output')
    subprocess.run('g++ -std=c++14 *.cpp -O3 ../std/*.cpp', shell=True)
    output_time = benchmark(subprocess.run, 1, './a.out', shell=True)
    print('Output code time:')
    print(output_time[1])
    os.chdir('..')

    os.chdir(directory)
    input_time = benchmark(subprocess.run, 1, 'python3 main', shell=True)
    print('Input code time:')
    print(input_time[1])

    '''
    # Cleanup
    cleardir('input')
    subprocess.run('touch input/.placeholder', shell=True)
    '''

if __name__ == '__main__':
    main()
