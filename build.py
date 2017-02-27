#!/usr/bin/env python

# Copyright Raoul Wols 2017
# Modified from original by Rene Rivera 2016 of github.com/boostorg/release-tools
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import httplib
import argparse
import sys
import inspect
import optparse
import os
import string
import time
import subprocess
import codecs
import shutil
import threading
import site
import tarfile
import multiprocessing
import urllib
import urllib2
# from urllib2 import urlopen # Python 2
#from urllib.request import urlopen # Python 3

class SystemCallError(Exception):
    def __init__(self, command, result):
        self.command = command
        self.result = result
    def __str__(self, *args, **kwargs):
        return "'%s' ==> %s"%("' '".join(self.command), self.result)

class utils:
    
    call_stats = []
    
    @staticmethod
    def call(*command, **kargs):
        utils.log( "%s> '%s'"%(os.getcwd(), "' '".join(command)) )
        t = time.time()
        result = subprocess.call(command, **kargs)
        t = time.time()-t
        if result != 0:
            print "Failed: '%s' ERROR = %s"%("' '".join(command), result)
        utils.call_stats.append((t,os.getcwd(),command,result))
        utils.log( "%s> '%s' execution time %s seconds"%(os.getcwd(), "' '".join(command), t) )
        return result
    
    @staticmethod
    def print_call_stats():
        utils.log("================================================================================")
        for j in sorted(utils.call_stats, reverse=True):
            utils.log("{:>12.4f}\t{}> {} ==> {}".format(*j))
        utils.log("================================================================================")
    
    @staticmethod
    def check_call(*command, **kargs):
        cwd = os.getcwd()
        # result = utils.call(*command, **kargs)
        result = subprocess.call(command, **kargs)
        if result != 0:
            raise SystemCallError([cwd].extend(command), result)
    
    @staticmethod
    def makedirs( path ):
        if not os.path.exists( path ):
            os.makedirs( path )
    
    @staticmethod
    def log_level():
        frames = inspect.stack()
        level = 0
        for i in frames[ 3: ]:
            if i[0].f_locals.has_key( '__log__' ):
                level = level + i[0].f_locals[ '__log__' ]
        return level
    
    @staticmethod
    def log( message ):
        sys.stdout.flush()
        sys.stderr.flush()
        sys.stderr.write( '# ' + '    ' * utils.log_level() +  message + '\n' )
        sys.stderr.flush()

    @staticmethod
    def rmtree(path):
        if os.path.exists( path ):
            #~ shutil.rmtree( unicode( path ) )
            if sys.platform == 'win32':
                os.system( 'del /f /s /q "%s" >nul 2>&1' % path )
                shutil.rmtree( unicode( path ) )
            else:
                os.system( 'rm -f -r "%s"' % path )

    @staticmethod
    def retry( f, max_attempts=5, sleep_secs=10 ):
        for attempts in range( max_attempts, -1, -1 ):
            try:
                return f()
            except Exception, msg:
                utils.log( '%s failed with message "%s"' % ( f.__name__, msg ) )
                if attempts == 0:
                    utils.log( 'Giving up.' )
                    raise

                utils.log( 'Retrying (%d more attempts).' % attempts )
                time.sleep( sleep_secs )



    @staticmethod
    def download_file(url):
        utils.check_call('wget', url)

    @staticmethod
    def make_file(filename, *text):
        f = codecs.open( filename, 'w', 'utf-8' )
        f.write( string.join( text, '\n' ) )
        f.close()
    
    @staticmethod
    def mem_info():
        if sys.platform == "darwin":
            utils.call("top","-l","1","-s","0","-n","0")
        elif sys.platform.startswith("linux"):
            utils.call("free","-m","-l")

class parallel_call(threading.Thread):
    
    def __init__(self, *command, **kargs):
        super(parallel_call,self).__init__()
        self.command = command
        self.command_kargs = kargs
        self.start()
    
    def run(self):
        self.result = utils.call(*self.command, **self.command_kargs)
    
    def join(self):
        super(parallel_call,self).join()
        if self.result != 0:
            raise(SystemCallError(self.command, self.result))

class BuildBase(object):

    def __init__(self, debug_level):
        self.debug_level = debug_level
        self.jobs = multiprocessing.cpu_count()
        self.root_dir = os.getcwd()
        self.build_dir = os.path.join(self.root_dir, 'build')
        utils.makedirs(self.build_dir)
        self.home_dir = os.path.expanduser('~')
        self.boost_version_major = 1
        self.boost_version_minor = int(os.getenv('BOOST_VERSION_MINOR', 60))
        self.build_shared_libs = os.getenv('BUILD_SHARED_LIBS', 'ON')
        self.build_type = os.getenv('CMAKE_BUILD_TYPE', 'Debug')
        self.boost_version_patch = 0
        self.boost_version = '{0}.{1}.{2}'.format(self.boost_version_major, self.boost_version_minor, self.boost_version_patch)
        self.boost_version_underscores = self.boost_version.replace('.','_')
        self.boost_dir = os.path.join(self.build_dir, 'boost_{}'.format(self.boost_version_underscores))

    def check_environment_variables(self):
        if not self.boost_version_minor:
            raise Exception('No boost_version_minor present.')
        if not self.build_type:
            raise Exception('No build_type present.')
        if not self.build_shared_libs:
            raise Exception('No build_shared_libs present.')

    def before_install(self):
        print('Changing directory to {}'.format(self.build_dir))
        os.chdir(self.build_dir)
        boost_tar_file = 'boost_{0}_{1}_{2}.tar.bz2'.format(self.boost_version_major, self.boost_version_minor, self.boost_version_patch)
        boost_url_prefix = 'https://downloads.sourceforge.net/project/boost/boost'
        url = boost_url_prefix + '/' + self.boost_version + '/' + boost_tar_file
        print('Downloading {}'.format(url))
        utils.download_file(url)
        print('Unpacking {}'.format(boost_tar_file))
        tar = tarfile.open(boost_tar_file, 'r:bz2')
        tar.extractall()
        tar.close()   

    def install(self):
        print('Changing directory to {}'.format(self.boost_dir))
        os.chdir(self.boost_dir)
        variant = 'debug' if self.build_type == 'Debug' else 'release'  
        variant = 'variant={}'.format(variant)
        link = 'shared' if self.build_shared_libs == 'ON' else 'static'
        link = 'link={}'.format(link)
        jobs = '-j{}'.format(str(self.jobs))
        libs =  ['system', 'serialization', 'test']
        if os.name == 'nt': # windows
            utils.check_call('bootstrap.bat')
            for i, lib in enumerate(libs):
                libs[i] = '--with-' + lib
            utils.check_call('./b2', '-d0', '-q', variant, link, jobs, *libs)
        else: # assume unix-like
            libs = '--with-libraries=' + ','.join(libs)
            utils.check_call('./bootstrap.sh', libs)
            utils.check_call('./b2', '-d0', '-q', variant, link, jobs)

    def before_build(self):
        print('Changing directory to {}'.format(self.build_dir))
        os.chdir(self.build_dir)
        utils.check_call('cmake', 
            self.root_dir, '-DCMAKE_SYSTEM_INCLUDE_PATH={}'.format(self.boost_dir), 
            '-DCMAKE_SYSTEM_LIBRARY_PATH={}'.format(self.boost_dir + '/stage/lib'), 
            '-DBUILD_SHARED_LIBS={}'.format(self.build_shared_libs),
            '-DCMAKE_BUILD_TYPE={}'.format(self.build_type))

    def build(self):
        print('Changing directory to {}'.format(self.build_dir))
        os.chdir(self.build_dir)
        utils.check_call('cmake', '--build', '.')

    def after_build(self):
        pass

    def before_test(self):
        pass

    def test(self):
        print('Changing directory to {}'.format(self.build_dir))
        os.chdir(self.build_dir)
        utils.check_call('ctest', '--output-on-failure')

    def after_test(self):
        pass

    def after_success(self):
        pass
    
    def after_failure(self):
        pass


class TravisBuild(BuildBase):

    def __init__(self, debug_level):
        super(TravisBuild, self).__init__(debug_level)

    def build(self):
        print('Changing directory to {}'.format(self.build_dir))
        os.chdir(self.build_dir)
        utils.check_call('make', '-j{}'.format(str(self.jobs)))

class AppveyorBuild(BuildBase):

    def __init__(self, debug_level):
        super(AppveyorBuild, self).__init__(debug_level)
        self.build_type = os.getenv('CONFIGURATION')

    def build(self):
        print('Changing directory to {}'.format(self.build_dir))
        os.chdir(self.build_dir)
        utils.check_call('cmake', '--build' ,'.' ,'--config', self.build_type)

class CommandLineBuild(BuildBase):

    def __init(self, debug_level):
        super(CommandLineBuild, self).__init__(debug_level)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--debug-level', dest='debug_level', type=int, default=0, help='controls the amount of debugging output printed')
    parser.add_argument('actions', nargs='+', type=str, help='actions to perform. can be at least one of: before-install, install, before-build, build, afer-build, before-test, test, after-test, after-success, after-failure')
    args = parser.parse_args()
    build = None
    if os.getenv('TRAVIS', False):
        build = TravisBuild(args.debug_level)
    elif os.getenv('APPVEYOR', False):
        build = AppveyorBuild(args.debug_level)
    else: # assume building locally from the command line
        build = CommandLineBuild(args.debug_level)

    if not args.actions:
        parser.print_help()
        return 1

    try:
        for action in args.actions:
            action = action.replace('-', '_')
            if hasattr(build, action):
                action_as_method = getattr(build, action)
                action_as_method()
            else:
                parser.print_help()
                return 2
    except Exception as e:
        print(str(e))
        return 3
    return 0

if __name__ == '__main__':
    exit_value = main()
    exit(exit_value)

