#!/usr/bin/env python
import os.path
import json
import glob

def get_long(json_files):
    threshold = 100
    longfun = {}
    for path in json_files:
        print "Reading {}".format(path)
        with open(path) as f:
            doc = json.load(f)
        for func in doc:
            length = func['line_end'] - func['line_start']
            if length > threshold:
                fun = "{}:{}".format(os.path.basename(func['file_name']), func['fun_name'])
                if exempt(func):
                    print "Skipping exempt function: {}".format(fun)
                else:
                    longfun[fun] = length
    return longfun

def exempt(func):
    # Check if a function has been annotated as exempt from the style check.
    with open(func['file_name']) as f:
        lines = f.readlines()
    line_start = func['line_start']
    # NOTE: line numbers start at 1, so line 2 is actually 1, prev being 0.
    line_prev = line_start - 2
    if line_prev >= 0:
        line = lines[line_prev]
        return "[STYLE-CHECK IGNORE LONG FUNCTION]" in line
    return False

longfun = get_long(glob.glob("*_metrics.json"))

with open('long_functions.json', 'w') as f:
    json.dump(longfun, f)
