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

