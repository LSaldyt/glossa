doubleSmallNumber x = if x > 100 then x else x*2   

a = [1, 2, 3]
b = [1,2..10]
c = [2*x | x <- b]
d = [ x | x <- c, x > 10]

t = (1, 2)

main = print 0
