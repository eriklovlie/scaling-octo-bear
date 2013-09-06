#!/usr/bin/env python
import os.path
import json
import glob

threshold = 100

longfun = {}

for path in glob.glob("*_metrics.json"):
    print "Reading {}".format(path)
    with open(path) as f:
        doc = json.load(f)
    for func in doc:
        length = func['line_end'] - func['line_start']
        if length > threshold:
            fun = "{}:{}".format(os.path.basename(func['file_name']), func['fun_name'])
            longfun[fun] = length

with open('long_functions.json', 'w') as f:
    json.dump(longfun, f)
