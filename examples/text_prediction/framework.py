import sys
import string
import random as rd
from functions import *

#Environment:
dictionary = dict()
iterations = 0
interactive = False
moveAhead = 0
quit = False
seed = rd.randint(0,1000000)

#Functions:

def usage():
    """Print out information regarding how to use this particular program."""
    print("Usage removed, sorry!")

def loadIntoDictionary(argv, dictionary):
    """Parse the argv array to a dictionary of strings->strings or 
    strings->string arrays. We assume all flags begin with a hyphen and all
    arguments to flags do not. So if we see -abc 1 2 3 then in the 
    dictionary we will get: dict['abc'] = ['1','2','3']
    """
    limit = len(argv)
    i = 0
    while i < limit:
        if argv[i].startswith('-'):
            j = i+1
            temp = []
            while j < limit and not argv[j].startswith('-'):
                temp.append(argv[j])
                j += 1
            if len(temp) == 1:
                dictionary[argv[i]] = temp[0]
            elif len(temp) > 1:
                dictionary[argv[i]] = temp
            else:
                usage()
                sys.exit()
            i = j-1
        i += 1
    return dictionary


def parseInput(argv):
    """So if you want to know how the pros deal with a main method 
    and inputs then check here:
    http://www.artima.com/weblogs/viewpost.jsp?thread=4829
    also: http://snipplr.com/view/2050/
    Or this: http://www.artima.com/forums/flat.jsp?forum=106&thread=4829
    But I tried those Then said, fuck this neck-beard bull shit, I'm doing 
    it my own way."""
    #This is important for if you're interacting with the program through 
    #the python interpreter or something. Don't ask me.
    if argv is None:
        argv = sys.argv
    #User SOS: Check for help.
    #If you want to print help and exit if no arguments are given then 
    #add the condition "or len(argv)==1"
    if '-h' in argv or '-help' in argv:
        print '\nUser SOS'
        usage()
        sys.exit(2)
    #Initialize dictionary
    dictionary = dict()
    #Check for a config file. Format it if found.
    if '-config' in argv:
        temp = argv.index('-config')+1
        try: #Try to open the file.
            f = open(argv[temp], 'r')
        except IOError as e:
            usage()
            sys.exit()
        contents = f.read()
        f.close()
        #Format the file, converting line breaks to blank spaces and 
        #then creating an array of strings.
        contents = contents.replace("\n",' ')
        contents = contents.strip()
        contents = contents.split(' ')
        #Add contents to the dictionary
        dictionary = loadIntoDictionary(contents, dictionary)
    #Next, add in command line options. Anything beginning with a hyphen, 
    #if it's already in the dictionary, replace the value in the dict with 
    #the command line args.
    return loadIntoDictionary(argv, dictionary)


def checkForInput(quit, interactive, moveAhead):
    global wordlist
    while True:
        inStr = raw_input('Waiting on user input... (Type: h for help)\n>')
        if inStr == 'q' or inStr == 'quit':
            quit = True; break
        elif inStr == '#' or inStr == 'num':
            temp=raw_input('Enter the number of generations to advance.\n>')
            moveAhead = int(temp); break
        elif inStr == 'f' or inStr == 'finish':
            interactive = False; break
        elif inStr == 'l' or inStr == 'limit':
            temp=raw_input('Enter the new word limit.\n>')
            global sentenceLengthLimit
            sentenceLengthLimit = int(temp)
        elif inStr == 'd':
            wordlist.getSentenceDeterministic()
        elif inStr == 'w':
            wordlist.getSentenceWeighted()
        elif inStr == 'u':
            wordlist.getSentenceUniform()
        else:
            print ''
    return quit, interactive, moveAhead

