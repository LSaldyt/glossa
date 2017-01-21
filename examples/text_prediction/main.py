#Run with
#python main.py -files testFile.txt
#for example

#Or for a more complicated run with:
#python main.py -files testFile.txt ~/Desktop/org.org -folders ~/Documents/Main/Writing/PowerOfThePen-Shorts-Beginnings/spaceOpera/ -extensions .org .txt

#This is a fun one too:
#python main.py -files ~/Documents/Main/Writing/PowerOfThePen-Shorts-Beginnings/spaceOpera/spaceopera.org

#For interactive mode:
#python main.py -files testFile.txt -i 1 -iters 100

"""Python script that takes a list of directories and a list of file extensions then it builds up a network of word frequencies and connections between words so that it can generate sentences, like so:
http://forums.xkcd.com/viewtopic.php?f=7&t=86012
This would be so fun.

pseudo code:
Find all documents of specified type.
For each document, open them up and split on any and all white space.
For each word
check if it ends in a period or a comma. For now, remove the commas.
If there's a period, strip off the period and make "." be the next "word" in the sentence (so that the connection between the current word and "." will be strengthened and the period will connect to the next word.)
Then check if the word exists in the dictionary. If so, increment its occurrence. If not, add it.
Then add a connection between the word and the subsequent word in the sentence.
Finally, create a list of words with no incoming links (to start sentences), and another list of words with incoming links from periods (to start sentences) and start running through generating random text.
"""

from functions import *
from framework import *
import os

def main(argv=None):
    global dictionary, iterations, interactive, moveAhead, quit, seed, wordlist, sentenceLengthLimit, rejected_words

    #Load all the command line and configuration file inputs into a dictionary
    dictionary = parseInput(argv)
    print '\nFor testing purposes, dictionary: '+str(dictionary)+'\n'

    #If any global variables are to be changed from their default values,
    #check for them in the dictionary and set them here.
    if '-i' in dictionary.keys():
        interactive = int(dictionary['-i'])
    dictionary['-i'] = interactive
    if '-iters' in dictionary.keys():
        iterations = int(dictionary['-iters'])
    dictionary['-iters'] = iterations
    if '-seed' in dictionary.keys():
        seed = int(dictionary['-seed'])
    dictionary['-seed'] = seed
    rd.seed(seed)

    #Get the files to read from
    files_to_process = []
    if '-files' in dictionary.keys():
        if isinstance(dictionary['-files'], str):
            files_to_process = [dictionary['-files']]
        else:
            files_to_process = dictionary['-files']
        dictionary['-files'] = files_to_process

    #Get the folders to search in
    folders = []
    if '-folders' in dictionary.keys():
        if isinstance(dictionary['-folders'], str):
            folders = [dictionary['-folders']]
        else:
            folders = dictionary['-folders']
        dictionary['-folders'] = folders

    #Get all the valid file extensions.
    extensions = []
    if '-extensions' in dictionary.keys():
        if isinstance(dictionary['-extensions'], str):
            extensions = [dictionary['-extensions']]
        else:
            extensions = dictionary['-extensions']
        dictionary['-extensions'] = extensions

    #Whether or not to make articles and subsequent nouns one unit.
    articlesSep = 1
    if '-articlesSep' in dictionary.keys():
        articlesSep = int(dictionary['-articlesSep'])

    #Get all the files to read in.
    for f in folders:
        for thisfile in os.listdir(f):
            if hasValidExtension(thisfile, extensions):
                files_to_process.append(f+thisfile)


    print '\nTESTING file list: '+str(files_to_process)+'\n'


    if len(files_to_process) <= 0:
        print '\nNo files could be found.\n'
        usage(); exit();


    #Process every file:
    for f in files_to_process:
        wordlist.addWords(processFile(f, articlesSep))

    #TESTING: print sorted accepted words:
    print '\nTESTING accepted words: '+str(sorted(wordlist.wordlist.keys()))+'\n'
    #TESTING: print sorted rejected words:
    print '\nTESTING rejected words: '+str(sorted(rejected_words))+'\n'


    wordlist.getSentenceDeterministic()
    wordlist.getSentenceWeighted()
    wordlist.getSentenceUniform()

    wordlist.getWordsByMostFrequentUsage()

    #Main loop
    for i in xrange(iterations):
        if interactive and moveAhead > 0:
            moveAhead = moveAhead - 1
        elif interactive:
            print 'Iteration: '+str(i)
            quit, interactive, moveAhead = checkForInput(quit, interactive, moveAhead)
        if quit:
            print 'Exiting.'
            exit()


if __name__ == "__main__":
    sys.exit(main())
