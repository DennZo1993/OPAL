#!/usr/bin/python

import argparse
import os
import subprocess


def parse_args():
  parser = argparse.ArgumentParser(description='Build OPAL')

  parser.add_argument('-d', '--debug',
                      action='store_true',
                      default=False,
                      help='Debug build')

  parser.add_argument('-r', '--release',
                      action='store_true',
                      default=False,
                      help='Release build')

  parser.add_argument('-t', '--test',
                      action='store_true',
                      default=False,
                      help='Run tests after successful build')

  parser.add_argument('-c', '--compiler',
                      default=None,
                      help='C++ compiler to use')

  parser.add_argument('--cov',
                      action='store_true',
                      default=False,
                      help='Collect coverage data')

  parser.add_argument('--clear',
                      action='store_true',
                      default=False,
                      help='Clear previous build data first')

  parser.add_argument('-v', '--verbose',
                      action='store_true',
                      default=False,
                      help='Verbose output')

  args = parser.parse_args()
  if args.debug == True and args.release == True:
    raise RuntimeError('Both debug and release flags set!')

  return args


def build(args):
  # Resolve build type.
  build_type = 'Coverage' if args.cov \
                          else 'Release' if args.release \
                                         else 'Debug'

  def goto_build_dir():
    import shutil

    BUILD_DIR = 'build'
    if args.clear:
      shutil.rmtree(BUILD_DIR, ignore_errors=True)

    if not os.path.exists(BUILD_DIR):
      os.makedirs(BUILD_DIR)

    os.chdir(BUILD_DIR)

  def run_cmake():
    cmake_cmd = ['cmake', '-DCMAKE_BUILD_TYPE=' + build_type]
    if args.compiler:
      cmake_cmd.append('-DCMAKE_CXX_COMPILER=' + args.compiler)
    cmake_cmd.append('..')

    subprocess.check_call(cmake_cmd)

  def run_make():
    subprocess.check_call(['cmake', '--build', '.', '--config', build_type])

  def run_tests():
    if args.test:
      cmd_list = ['ctest', '-C', build_type]
      if args.verbose:
        cmd_list.append('--verbose')

      subprocess.check_call(cmd_list)

  goto_build_dir()
  run_cmake()
  run_make()
  run_tests()


def main():
  args = parse_args()

  build(args)


if __name__ == '__main__':
  import sys
  sys.exit(main())
