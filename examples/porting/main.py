import sys

def f(x):
    print x / 2
    raise ValueError, "dodgy value"

if __name__ == "__main__":
    print "Hello World",

    try:
        f(3) # Prints 3..
    except Exception, e:
        print('Caught an exception')

    heights = {'tom' : 6, 'sally' : 5}
    for key in heights.iterkeys():
        print key
    for value in heights.itervalues():
        print value
    for item in heights.iteritems():
        print item

