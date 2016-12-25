
quicksort [] = []
quicksort l  = [ a | a <- l, a <= l !! 0 ] ++ [ l !! 0 ] ++ [a | a <- l, a > l !! 0]

a = [5, 4, 3, 2, 1, 2, 3, 4, 5]
b = quicksort a

main = do
    let b = quicksort a
    print 0
