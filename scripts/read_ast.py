#!/usr/bin/env python3
from pprint import pprint

def get_indent(line):
    i = 0
    for c in line:
        if c == ' ':
            i += 1
        else:
            break
    return i // 4

def build_tree(content):
    tree = []
    multisymbol = ''
    current_subcontent = []
    for indent, line in content:
        if line.startswith('MultiSymbol') and indent == 0:
            if multisymbol != '':
                tree.append((multisymbol, build_tree(current_subcontent)))
                #current_subcontent = []
            multisymbol = line
        else:
            if multisymbol == '':
                tree.append(line)
            else:
                current_subcontent.append((indent - 1, line))
    return tree

def read_ast(filename):
    with open(filename, 'r') as astfile:
        content = [line for line in astfile if len(line) > 1]
    content = [(get_indent(line), line.replace('    ', '')) for line in content]
    tree = build_tree(content)
    pprint(tree)

if __name__ == '__main__':
    read_ast('examples/asts/main.ast')
