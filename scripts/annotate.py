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

def annotate(filename):
    annotated  = []
    lastIndent = 0

    with open(filename, 'r') as unannotated:
        content = [line for line in unannotated]

    for line in content:
        if line == '\n':
            continue
        print(line)
        indent = get_indent(line)
        print('indent: %s' % indent)
        if indent < lastIndent:
            diff = lastIndent - indent
            print('diff: %s' % diff)
            for i in range(diff):
                annotated.append('end')
        annotated.append(line)
        lastIndent = indent

    for i in range(lastIndent):
        annotated.append('end')

    with open(filename, 'w') as output:
        output.write('\n'.join(annotated))
