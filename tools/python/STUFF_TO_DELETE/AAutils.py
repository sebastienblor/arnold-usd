import os
import glob
from build_tools import *
from SCons.Script import *



def list_tests(target, source, env):
    for file in source:
        name = os.path.basename(os.path.dirname(str(file)))
        f = open(os.path.join(str(file)), 'r')
        summary = f.readline().strip('\n')
        f.close
        print '%s: %s' % (name, summary)
    return None

def list_test_errors(target, source, env):
    for file in source:
        dirname = os.path.dirname(str(file))
        name = os.path.basename(dirname)
        f = open(str(file), 'r')
        value = f.readline().strip('\n')
        if value != 'OK':
            print '%s: %s' % (name, value)
            l = open(os.path.join(dirname, name + '.log'), 'r')
            while True:
                line = l.readline().strip('\n')
                if line == "":
                    # We have reached the end of file.
                    break
                if (line.lower().find('error') != -1) or (line.lower().find('warning') != -1):
                    print line
            l.close()
        f.close
    return None

## Creates a new test in the testsuite directory
def make_test(target, source, env):
    testpath = str(target[0])
    testname = os.path.basename(testpath)
    os.mkdir(testpath)
    os.mkdir(os.path.join(testpath, 'data'))
    os.mkdir(os.path.join(testpath, 'ref'))

    # Write README file
    f = open(os.path.join(testpath, 'README'), 'w')
    f.write('''****TODO**** write a one-line description for test case %s

author: --''' % testname)
    f.close()

    return None ## always succeeds


## Finds a test group and returns a list of all tests included
def find_test_group(group):
    def find_test_group_in_file(group, file):
        TEST_GROUP = []
   
        f = open(file, 'r')
        for line in f.readlines():
            line = line.lstrip(' \t')
            if line.startswith('#'):
                # Skip comment lines
                continue
            (l, s, r) = strpartition(line, ':')
            if s == ':':
                if group == l.rstrip():
                    # We found the test group
                    TEST_GROUP += Split(r)
                    index = 0
                    while True:
                        if index == len(TEST_GROUP):
                            break
                        test = TEST_GROUP[index]
                        if not test.startswith('test_'):
                            # We found a nested group, expand it into single tests
                            TEST_GROUP += find_test_group(test)
                            del TEST_GROUP[0]
                        else:
                            index += 1
                    break
        f.close()
        return TEST_GROUP

    TEST_GROUP = []
   
    # First search the user local file for this group (only if the local file exists)
    if os.path.exists(os.path.join('testsuite', 'groups.local')):
        TEST_GROUP = find_test_group_in_file(group, os.path.join('testsuite', 'groups.local'))
      
    # If not found, then search the global test groups mapping file
    if len(TEST_GROUP) == 0:
        TEST_GROUP = find_test_group_in_file(group, os.path.join('testsuite', 'groups'))
   
    return TEST_GROUP

## Gets the next test name from the list of existing tests (assuming a 'test_0000' format)
def get_next_test_name():
    l = glob.glob(os.path.join("testsuite", 'test_[0-9][0-9][0-9][0-9]'))
    l.sort()
    result = int(os.path.basename(l[len(l) - 1])[5:]) + 1
    return "%04d" % result