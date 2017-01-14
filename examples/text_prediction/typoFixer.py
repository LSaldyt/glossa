import sys

'''This is to be run independently of any of the other scripts in this folder. Simply pass it two arguments, the name of a file to read in and the name of a file to write the results out to and this script will make a large number of common typo corrections to the input file and write them to the output file.
Example usage follows:
> python typoFixer.py input.txt output.txt
Example usage follows:
> python typoFixer.py ~/Desktop/org.org ~/Desktop/deleteme.txt
'''

infile = sys.argv[1]
outfile = sys.argv[2]

typos = [
('compells', 'compels'),
('fprward', 'forward'),
('Moreland', 'Morland'),
('Thhe ', 'The '),
('nothingg', 'nothing'),
('bankrupcy', 'bankruptcy'),
('morland', 'Morland'),
(' veery ', ' very '),
('ocupies', 'occupies'),
('acquiesecne', 'acquiescence'),
('differenr ', 'different '),
('differeny ', 'different '),
('emmisary', 'emissary'),
(' hm ', ' him '),
('mirrior', 'mirror'),
('carpetted', 'carpeted'),
('inscence', 'incense'),
('argumnets', 'arguments'),
('agaisnt', 'against'),
('honot', 'honor'),
('wwill', 'will'),
('objet', 'object'),
('necesary', 'necessary'),
('', ''),
('', ''),
('', ''),
('', ''),
('', ''),
('', ''),
('', ''),
]

infilehandle = open(infile, 'r')
outfilehandle = open(outfile, 'w')
for line in infilehandle:
    for t in typos:
        line = line.replace(t[0], t[1])
    outfilehandle.write(line)
infilehandle.close()
outfilehandle.close()
