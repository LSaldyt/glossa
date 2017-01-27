#Functions and some global variables were moved here simply to clean up main.py.

import re #import regular expressions
import string
import obj_wordlist


#Limit on the length of generated sentences.
#TODO. Later you could do a depth-limited, depth-first search for a path to a period to end the sentence.
sentenceLengthLimit = 20


#Word list - store all words and connections here
wordlist = obj_wordlist.WordList()


#Keep track of rejected words for testing purposes
rejected_words = []


#For now, use a regular expression to match only words containing
#alpha-numeric characters, hyphens, or apostrophes.
alpha = re.compile("^[\w\'-]+$")

#Match articles
articles = re.compile('a|an|the')

def processFile(filetoreadin, articlesSep):
    """Takes a filename and returns an array of strings ready to be fed to the wordlist"""
    global alpha, rejected_words, articles
    to_return = []
    file_handle=open(filetoreadin)
    line = file_handle.readline()
    article = '' #This will be set if an article is found.
    while line:
        line = string.lower(line) #lower case the whole string.
        words = line.split(' ')
        for i in xrange(len(words)):
            w = words[i]
            #Remove white space
            w = w.strip()

            #Deal with commas
            if w.endswith(","):
                w = w.replace(",","")
            #Deal with periods
            endsWithPeriod = False
            if w.endswith("."):
                w = w.replace(".","")
                endsWithPeriod = True

            #for now, remove anything that is not purely alpha-numeric
            result = alpha.match(w)
            if result is None:
                if not w in rejected_words:
                    rejected_words.append(w)
            else:
                result = articles.match(w)
                if not articlesSep and not result is None:
                    article = w
                else:
                    if article:
                        to_return.append(article+' '+w)
                        article = ''
                    else:
                        to_return.append(w)
            if endsWithPeriod:
                to_return.append('.')
        line = file_handle.readline()
    file_handle.close()
    return to_return


def hasValidExtension(f, extensions):
    for e in extensions:
        if f.endswith(e):
            return True
    return False

