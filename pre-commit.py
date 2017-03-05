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
import shutil

root_dir = subprocess.check_output(['git', 'rev-parse' ,'--show-toplevel']).decode('utf-8').strip()
sys.path.append(root_dir)
sys.dont_write_bytecode = True

import build

def print_msg(msg):
    print('\n\t{}\n'.format(msg))

def get_changed_files():
    os.chdir(root_dir)
    return subprocess.check_output(['git', 'diff-index', '--cached', '--name-only', 'HEAD']).decode('utf-8').splitlines()

class GitStasher(object):
    """Pushes and pops unstaged changes in a Python "with" block"""
    def __init__(self):
        super(GitStasher, self).__init__()

    def __enter__(self):
        try:
            os.chdir(root_dir)
            subprocess.check_call(['git', 'stash', 'save', '--keep-index', '--quiet'])
        except subprocess.CalledProcessError as e:
            print_msg('Failed to temporarily stash unstaged changes!\n\t(return code {})'.format(str(e.returncode)))
            raise Exception('Fatal error.')

    def __exit__(self, exception_type, exception_value, traceback):
        try:
            os.chdir(root_dir)
            subprocess.check_call(['git', 'stash', 'pop', '--quiet'])
        except subprocess.CalledProcessError as e:
            print_msg('Failed to pop unstaged changes!\n\t(return code {})'.format(str(e.returncode)))
        except Exception as e:
            print_msg('Failed to pop unstaged changes! (Unknown exception)')

def do_clang_format():
    if not shutil.which('clang-format'):
        print_msg('clang-format not present, skipping formatting.')
        return
    os.chdir(root_dir)
    for file in get_changed_files():
        if not f.endswith(('.h','.c', '.hh', '.cc', '.hpp', '.hpp', '.hxx', '.cxx')):
            continue
        try:
            print_msg('Formatting "{}"'.format(file))
            subprocess.check_call(['clang-format', '-style=file', '-i', file])
        except Exception as e:
            print_msg('Failed formatting, but continuing on.')


def do_build():
    # Run clang-format on the changed files, if present.
    print_msg('Building and testing the project.')
    b = build.CommandLineBuild(0)
    if b.before_build():
        b.after_failure()
        return 1
    elif b.build():
        b.after_failure()
        return 2
    elif b.after_build():
        b.after_failure()
        return 3
    elif b.before_test():
        b.after_failure()
        return 4
    elif b.test():
        b.after_failure()
        return 5
    elif b.after_test():
        b.after_failure()
        return 6
    else:
        b.after_success()
        return 0

def main():
    exit_status = 0
    try:
        with GitStasher() as stash:
            exit_status = do_build()
    except build.SystemCallError as e:
        exit_status = -1
    except Exception as e:
        exit_status = -2
    if exit_status != 0:
        print_msg('There were build and/or test errors.\n\tPlease fix them and then try committing again.')
    return exit_status

if __name__ == '__main__':
    exit(main())
