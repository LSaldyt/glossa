
def chunkify(lst, n):
    return [ lst[i::n] for i in range(n) ]

def set_array(shape, b):
    print('setting array')
    if len(shape) == 1:
        return b

    dim = shape[0]
    sub_bs = chunkify(b, dim)
    return [sub_bs[i] for i in range(dim)]
