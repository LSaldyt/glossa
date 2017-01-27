from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals
from past.utils import old_div
from __pylib__  import *
from obj_word import * 
from string import * 
class WordList():
    def __init__ (self,limit=20): 
        self.wordlist = dict()
        """Add period to the word list.""" 
        self.wordlist["."] = obj_word.WordNode() 
        self.sentenceLengthLimit = limit 
        self.currentWord = "."  
    def addWords (self,words): 
        previous_word = "." 
        for i in range(len(words)): 
            w = words[i] 
            try:
            
                self.wordlist[w].incrementCount() 
            
            except KeyError: 
                self.wordlist[w] = obj_word.WordNode()   
    """
                #If there is a previous word.
    
                if i != 0:
    
                    #Add this word as outgoing from the previous word.
    
                    self.wordlist[previous_word].addOutgoing(w)
    
                    #Add the previous word as incoming to this word.
    
                    self.wordlist[w].addIncoming(previous_word)
    
    end
                else:
    
                    #Add period as outgoing to this word to 
    
                    #indicate that this word can begin a sentence.
    
                    self.wordlist['.'].addOutgoing(w)
    
                    self.wordlist[w].addIncoming('.')
    
    end
                previous_word = w
    
    end
            #If the very last word is not a period, it should be
    
            if previous_word != '.':
    
                self.wordlist[previous_word].addOutgoing('.')
    
                self.wordlist['.'].addIncoming(previous_word)
    
    end
    end
        def getNextWord(self, howToGet):
    
            if howToGet == 'd':
    
                self.currentWord = self.wordlist[self.currentWord]\
    
                .getNextWordDeterministic()
    
    end
            elif howToGet == 'w':
    
                self.currentWord = self.wordlist[self.currentWord]\
    
                .getNextWordWeighted()
    
    end
            elif howToGet == 'u':
    
                self.currentWord = self.wordlist[self.currentWord]\
    
                .getNextWordUniform()
    
    end
            else:
    
                print 'ERROR in obj_wordlist.py.'; exit()
    
    end
            return self.currentWord
    
    end
        def getSentence(self, howToGet, sentence_type):
    
            print '\nA '+sentence_type+' sentence: '
    
            word = self.getNextWord(howToGet)
    
            sentence = string.capitalize(str(word)+' ')
    
            count = 0
    
            while word != '.' and count < self.sentenceLengthLimit:
    
                word = self.getNextWord(howToGet)
    
                sentence += word+' '
    
                count+=1
    
    end
            print sentence
    
            print 'Counted to '+str(count)
    
            self.currentWord = '.' #Reset to sentence start.
    
    end
        def getSentenceDeterministic(self):
    
            self.getSentence('d', 'deterministic')
    
    end
        def getSentenceWeighted(self):
    
            self.getSentence('w', 'weighted random')
    
    end
        def getSentenceUniform(self):
    
            self.getSentence('u', 'uniform random')
    
    end
        def getWordsByMostFrequentUsage(self):
    
            s = sorted(self.wordlist.keys(), key=lambda word: self.wordlist[word].count)
    
            for word in s:
    
                print word+' - '+str(self.wordlist[word].count)
    
    end
    end
        """  
