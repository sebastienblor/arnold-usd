#!/usr/bin/env python
import sys
import codecs
import os

def fix(filename):
    print "fixing", filename
    text = None
    with codecs.open(filename, 'r', encoding='utf-8') as f:
        text = f.read()
        text = text.encode('ascii', 'ignore')
    if text is not None:
        with codecs.open(filename, 'w', encoding='ascii') as f:
            f.write(text)

def main(root):
    for root, dirs, files in os.walk(root):
        for f in files:
            if f.endswith('.cpp') or f.endswith('.h'):
                fix(os.path.join(root, f))

if __name__ == '__main__':
    if len(sys.argv) <= 1:
        root = '.'
    else:
        root = sys.argv[1]
    main(root)

