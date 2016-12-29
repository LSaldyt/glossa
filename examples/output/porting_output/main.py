from past.utils import old_div
from sys import *  
def fileprint ( ) : 
     print( "Hello World"   , file= sys . stderr   )   
def exception ( ) : 
     raise ValueError  ( "Weird value"  ) .with_traceback()   
def division ( ) : 
     assert old_div( 2 , ( 1  + 2  )  )   == 0    
if __name__ == "__main__" :
     print( "Hello World"   , end='')   
