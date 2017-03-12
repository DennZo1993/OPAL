#!/usr/bin/python

import argparse
import os
import subprocess


def parse_args():
  parser = argparse.ArgumentParser(description='Build OPAL')

  parser.add_argument('-d', '--debug',
                      action='store_true',
                      default=False)

  parser.add_argument('-r', '--release',
                      action='store_true',
                      default=False)

  parser.add_argument('-t', '--test',
                      action='store_true',
                      default=False)

  parser.add_argument('-c', '--compiler',
                      default='clang++')

  parser.add_argument('--cov',
                      action='store_true',
                      default=False)

  args = parser.parse_args()
  if args.debug == True and args.release == True:
    raise RuntimeError('Both debug and release flags set!')

  return args


def build(args):
  def goto_build_dir():
    BUILD_DIR = 'build'
    if not os.path.exists(BUILD_DIR):
      os.makedirs(BUILD_DIR)

    os.chdir(BUILD_DIR)

  def run_cmake():
    cmake_cmd = ['cmake']
    build_type = 'Coverage' if args.cov \
                            else 'Release' if args.release \
                                           else 'Debug'

    cmake_cmd.append('-DCMAKE_BUILD_TYPE=' + build_type)
    if args.compiler:
      cmake_cmd.append('-DCMAKE_CXX_COMPILER=' + args.compiler)
    cmake_cmd.append('..')

    subprocess.check_call(cmake_cmd)

  def run_make():
    subprocess.check_call(['make', '-j4'])

  def run_tests():
    if args.test:
      subprocess.check_call(['ctest', '--verbose'])


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
