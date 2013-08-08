#!/usr/bin/env python
import os.path
import yaml
import json

threshold = 100

with open('metrics.yaml') as f:
    doc = yaml.load(f)
    longfun = {}
    for func in doc:
        length = func['line_end'] - func['line_start']
        if length > threshold:
            fun = "{}:{}".format(os.path.basename(func['file_name']), func['fun_name'])
            longfun[fun] = length
    with open('long_functions.json', 'w') as f:
        json.dump(longfun, f)
