#!/usr/bin/env python
#
# This pre-commit hook builds and tests the project. If anything went wrong, 
# then we're not going to commit.
# 
# To enable this git hook, run the command
# 
#     ln -s ../../pre-commit.py .git/hooks/pre-commit

import sys
import subprocess
import os
root_dir = subprocess.check_output(['git', 'rev-parse' ,'--show-toplevel']).decode('utf-8').strip()
sys.path.append(root_dir)
sys.dont_write_bytecode = True
import build

def print_msg(msg):
    print('\n\t{}\n'.format(msg))

class GitStasher(object):
    """Pushes and pops unstaged changes in a Python "with" block"""
    def __init__(self):
        super(GitStasher, self).__init__()

    def __enter__(self):
        try:
            os.chdir(root_dir)
            subprocess.check_call(['git', 'stash', 'save', '--keep-index'])
        except subprocess.CalledProcessError as e:
            print_msg('Failed to temporarily stash unstaged changes!\n\t(return code {})'.format(str(e.returncode)))
            raise Exception('Fatal error.')

    def __exit__(self, exception_type, exception_value, traceback):
        try:
            os.chdir(root_dir)
            subprocess.check_call(['git', 'stash', 'pop'])
        except subprocess.CalledProcessError as e:
            print_msg('Failed to pop unstaged changes!\n\t(return code {})'.format(str(e.returncode)))
        except Exception as e:
            print_msg('Failed to pop unstaged changes! (Unknown exception)')

def do_build():
    b = build.CommandLineBuild(0)
    if b.before_build():
        return 1
    elif b.build():
        return 2
    elif b.after_build():
        return 3
    elif b.before_test():
        return 4
    elif b.test():
        return 5
    elif b.after_test():
        return 6
    else:
        # OK!
        return 0

def main():
    try:
        with GitStasher() as stash:
                return do_build()
    except build.SystemCallError as e:
        print_msg('There were build and/or test errors.\n\tPlease fix them and then try committing again.')
        return -1
    except Exception as e:
        print_msg(str(e))
        return -2

if __name__ == '__main__':
    exit(main())
