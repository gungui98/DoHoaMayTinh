# Simple python script which recursively goes through all of the inputs
# in /svg/ and runs the draw binary on each one, comparing the output
# to the master copy. Should be easy to add inputs beyond those in /svg/

import os
import subprocess
import filecmp

draw_binary = os.path.join('..', '..', 'build', 'draw')

# Visual studio
if not os.path.isfile(draw_binary):
    draw_binary = os.path.join('..', '..', 'build', 'Debug', 'draw')

scan_dirs = [os.path.join('..', '..', 'svg')] # Add more directories here with .svg files
output_dir = 'outputs/'
reference_dir = 'reference/'

print(draw_binary)

def compare(output, reference):
    # For now, just do a binary diff. May want to use a per-pixel comparison
    # instead.

    return filecmp.cmp(output, reference)

if not os.path.exists(output_dir):
    os.makedirs(output_dir)

all_tests = []

for scan_dir in scan_dirs:
    for root, subdirs, files in os.walk(scan_dir):
        for f in files:
            all_tests.append(os.path.join(root, f))

print('Found {} tests'.format(len(all_tests)))

def to_filename(path):
    retval = ''
    for x in path:
        if x in 'abdefghijklmnopqrstuvwxyz0123456789':
            retval += x
        elif len(retval) > 0:
            retval += '-'
    return retval + '.png'

for test_input in all_tests:
    print(test_input)
    fname = to_filename(test_input)
    subprocess.call([draw_binary, test_input, 'nogl', '1024', '1024'])
    output_fname = os.path.join(output_dir, fname)
    if os.path.isfile(output_fname):
        os.remove(output_fname)
    os.rename('test.png', output_fname)

# Check that all are the same:
print('Verifying correctness...')

passes = 0
failures = 0
for test_input in all_tests:
    print(test_input)
    fname = to_filename(test_input)
    output = os.path.join(output_dir, fname)
    reference = os.path.join(reference_dir, fname)

    are_equal = compare(output, reference)

    if are_equal:
        print('PASS')
        passes += 1
    else:
        print('FAIL')
        failures += 1
print('====')
print('{} passed. {} failed.'.format(passes, failures))






