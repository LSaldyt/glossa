#!/usr/bin/env python3
import sys

def get_indent(line):
    i = 0
    for c in line:
        if c == ' ':
            i += 1
        elif c == '\t':
            i += 4
        else: 
            break
    return i // 4;

filename   = sys.argv[1]
annotated  = []
lastIndent = 0

with open(filename, 'r') as content:
    for line in content:
        indent = get_indent(line)
        annotated.append(line)
        if indent < lastIndent:
            annotated.append('end')
        lastIndent = indent

if lastIndent > 0:
    annotated.append('end')

with open(filename, 'w') as output:
    output.write('\n'.join(annotated))
