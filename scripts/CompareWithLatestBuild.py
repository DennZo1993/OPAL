#!/usr/bin/python3

import os
import subprocess
import shutil
import json

LATEST_VERSION_SAVE_PATH = 'latest_version'
PROJECT_NAME = 'OPAL'
PROJECT_URL = 'https://github.com/DennZo1993/OPAL.git'

DST_PATH = os.path.join(LATEST_VERSION_SAVE_PATH, PROJECT_NAME)


def checkout_latest_version():
  print('Checking out latest version...')

  current_path = os.getcwd()

  def needs_clone():
    if not os.path.exists(DST_PATH):
      return True
    os.chdir(DST_PATH)
    url = subprocess.check_output(['git', 'config', '--get', 'remote.origin.url'])
    os.chdir(current_path)

    return url == PROJECT_URL

  if needs_clone():
    print('Cloning latest version...')

    shutil.rmtree(LATEST_VERSION_SAVE_PATH, ignore_errors=True)
    os.makedirs(LATEST_VERSION_SAVE_PATH)
    os.chdir(LATEST_VERSION_SAVE_PATH)
    subprocess.check_call(['git', 'clone', PROJECT_URL])
    os.chdir(PROJECT_NAME)
  else:
    print('Project version already exists')
    print('Updating latest version...')

    os.chdir(DST_PATH)
    subprocess.check_call(['git', 'pull'])

  os.chdir(current_path)


def build_latest_version():
  print('Building latest version...')

  current_path = os.getcwd()

  os.chdir(DST_PATH)
  import subprocess
  subprocess.check_call(['./opal_build.py', '-r', '-c', 'g++'])
  os.chdir(current_path)


def run_test(directory):
  current_path = os.getcwd()
  os.chdir(directory)
  subprocess.call('./RunSegmentationWithAllBase.py')
  with open('results.json', 'r') as f:
    results = json.load(f)
  os.chdir(current_path)
  return results


def main():
  current_path = os.getcwd()

  checkout_latest_version()
  build_latest_version()

  print(os.getcwd())
  # Run latest version.
  latest_results = run_test(os.path.join(DST_PATH, 'scripts'))

  print(current_path)
  # Run workig copy.
  latest_results = run_test(current_path)


if '__main__' == __name__:
  import sys
  sys.exit(main())
