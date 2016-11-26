import os
import re
import subprocess
import json
from shutil import rmtree

from config import *
from Common import *


global_times = []
global_scores = {}


def run_segmentation(config, images, segmentations, target_idx):
  '''
  OPAL <config_file>
       <target_image> <ground_truth_seg>
       <base_image_1> <base_seg_1>
       ...
       <base_image_n> <base_seg_n>
       <result_dir>
  '''
  args = [OPAL_EXE, config,
          os.path.join(IMAGES_BASE, images[target_idx]),
          os.path.join(SEGMENTATION_BASE, segmentations[target_idx])
         ]

  current_results_dir = os.path.join(RESULTS_ROOT_DIR, str(target_idx))
  os.mkdir(current_results_dir)

  for i in range(len(images)):
    if i == target_idx:
      continue
    args += [os.path.join(IMAGES_BASE, images[i]),
             os.path.join(SEGMENTATION_BASE, segmentations[i])]

  args.append(current_results_dir)

  current_log_file = os.path.join(current_results_dir, 'log.txt')
  with open(current_log_file, 'w') as log:
    subprocess.call(args, stdout=log)

  get_results_from_log(current_log_file, global_scores, global_times)


def main():
  if not os.path.exists(OPAL_EXE):
    raise ValueError('Path to OPAL executable "{s}" is incorrect!'.format(s=OPAL_EXE))

  images = get_images()
  segmentations = get_segmentations()

  if (len(images) != len(segmentations)):
    raise ValueError('Images count {i} != segmentations count {j}'.format(i=len(images), j=len(segmentations)))

  rmtree(RESULTS_ROOT_DIR, ignore_errors=True)
  os.mkdir(RESULTS_ROOT_DIR) 

  opal_config = os.path.join(RESULTS_ROOT_DIR, 'config.txt')
  make_default_opal_config(opal_config)

  print('Running OPAL...')
  for i in range(len(images)):
    run_segmentation(opal_config, images, segmentations, i)
    print('Image {i} done'.format(i=i))

  results = {}
  results['times'] = global_times
  results['scores'] = global_scores
  with open('results.json', 'w') as f:
    json.dump(results, f, sort_keys=True, indent=4, separators=(',', ':'))

  results_stats(global_times, global_scores)


if '__main__' == __name__:
  main()
