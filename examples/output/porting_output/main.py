from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals
from past.utils import old_div
from sys import *  
def f (x): 
    print(old_div(x, 2))  
    raise ValueError("dodgy value")    
if __name__ == "__main__":
    print("Hello World", end='')  
    try:
        f(3) # Prints 3.. 
    except Exception as e:
        print(("Caught an exception"))    
    heights = {
    "tom" : 6,"sally" : 5
    }  
    for key in heights: 
        print(key)    
    for value in heights.values(): 
        print(value)    
    for item in heights.items(): 
        print(item)      
