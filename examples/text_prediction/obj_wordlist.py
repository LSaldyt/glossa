import obj_word
import string

class WordList():
    def __init__(self, limit=20):
        self.wordlist = dict()
        #Add period to the word list.
        self.wordlist['.'] = obj_word.WordNode()

        self.sentenceLengthLimit=limit

        self.currentWord = '.'


    def addWords(self, words):
        previous_word = '.'
        for i in xrange(len(words)):
            w = words[i]
            try:
                self.wordlist[w].incrementCount()
            except KeyError:
                self.wordlist[w] = obj_word.WordNode()
            #If there is a previous word.
            if i != 0:
                #Add this word as outgoing from the previous word.
                self.wordlist[previous_word].addOutgoing(w)
                #Add the previous word as incoming to this word.
                self.wordlist[w].addIncoming(previous_word)
            else:
                #Add period as outgoing to this word to 
                #indicate that this word can begin a sentence.
                self.wordlist['.'].addOutgoing(w)
                self.wordlist[w].addIncoming('.')
            previous_word = w
        #If the very last word is not a period, it should be
        if previous_word != '.':
            self.wordlist[previous_word].addOutgoing('.')
            self.wordlist['.'].addIncoming(previous_word)

    def getNextWord(self, howToGet):
        if howToGet == 'd':
            self.currentWord = self.wordlist[self.currentWord].getNextWordDeterministic()
        elif howToGet == 'w':
            self.currentWord = self.wordlist[self.currentWord].getNextWordWeighted()
        elif howToGet == 'u':
            self.currentWord = self.wordlist[self.currentWord].getNextWordUniform()
        else:
            print 'ERROR in obj_wordlist.py.'; exit()
        return self.currentWord


    def getSentence(self, howToGet, sentence_type):
        print '\nA '+sentence_type+' sentence: '
        word = self.getNextWord(howToGet)
        sentence = string.capitalize(str(word)+' ')
        count = 0
        while word != '.' and count < self.sentenceLengthLimit:
            word = self.getNextWord(howToGet)
            sentence += word+' '
            count+=1
        print sentence
        print 'Counted to '+str(count)
        self.currentWord = '.' #Reset to sentence start.

    def getSentenceDeterministic(self):
        self.getSentence('d', 'deterministic')

    def getSentenceWeighted(self):
        self.getSentence('w', 'weighted random')

    def getSentenceUniform(self):
        self.getSentence('u', 'uniform random')

    def getWordsByMostFrequentUsage(self):
        s = sorted(self.wordlist.keys(), key=lambda word: self.wordlist[word].count)
        for word in s:
            print word+' - '+str(self.wordlist[word].count)
