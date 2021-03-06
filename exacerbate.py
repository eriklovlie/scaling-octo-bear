#!/usr/bin/env python

"""

Check if a problem of long functions has been exacerbated.

Useful for "legacy" code bases, to have the quality of the code move in the right direction without
having to do a massive cleanup first.

Input:
  - an initial set of long functions, created once and never added to.
  - the current set of long functions

Algorithm:
  - read a saved state of the long functions (the length of each is the lowest recorded)
  - read the current set of long functions
  - for any current function not in the old set:
    - report warning
  - for any current function which is also in the old set, but is now longer:
    - report warning
  - for any current function which is also in the old set, but is now shorter:
    - record the new size
  - for any old function which no longer appears in the current set:
    - remove from old set.
    - this will prevent this function from exceeding the threshold in the future.

Output:
  - a potentially updated (for functions which have shrunk) set of long functions.
  - information to stdout about new issues and any improvements

A consequence of this algorithm is that you can split a long function in several new functions,
as long as each new function is below the threshold.

"""

import os.path
import json
import operator

def current_long_functions():
    with open('long_functions.json') as f:
        doc = json.load(f)
    return doc

def is_exacerbated():
    with open('long_functions_old.json') as f:
        old = json.load(f)
    exacerbated = False
    current = current_long_functions()
    for fun, length in current.items():
        if not fun in old:
            exacerbated = True
            print "ERROR: New long function {} ({} lines).".format(fun, length)
        else:
            if length > old[fun]:
                exacerbated = True
                print "ERROR: Exacerbated long function {} (previously {} lines, now {}).".format(fun, old[fun], length)
            elif length < old[fun]:
                print "INFO: Long function {} made shorter (previously {} lines, now {}).".format(fun, old[fun], length)
                old[fun] = length
    fixed = [fun for fun in old if fun not in current]
    for fun in fixed:
        print "INFO: Fixed or removed {} (previously {} lines)".format(fun, old[fun])
        del old[fun]
    with open('long_functions_old.json', 'w') as f:
        json.dump(old, f)
    print "Current set of long functions:"
    for name, length in sorted(old.iteritems(), key=operator.itemgetter(1)):
	print "{:>6}: {}".format(length, name)
    return exacerbated

if __name__ == "__main__":
    print "Running check for bad changes."
    if is_exacerbated():
        print "Yep, made worse."
        exit(1)
    print "Nope, we are no worse than before."
