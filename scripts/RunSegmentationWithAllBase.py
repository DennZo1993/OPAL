import os
import re
import subprocess
import json
from shutil import rmtree

from config import make_opal_config


OPAL_EXE = '../build/main/OPAL'
IMAGES_BASE = '../test/test_data/Images'
SEGMENTATION_BASE = '../test/test_data/Segmentations'
RESULTS_ROOT_DIR = 'results'

global_times = []
global_scores = {}


def get_results_from_log(target_idx, scores, times):
  dice_scores = []
  running_time = ''

  log_file = os.path.join(RESULTS_ROOT_DIR, str(target_idx), 'log.txt')
  with open(log_file, 'r') as log:
    for l in log:
      line = l.rstrip()
      if re.search('[0-9]+\t[0-9]+(\.[0-9]+)?', line):
        dice_scores.append(line)
      elif 'OPAL running time:' in line:
        _, running_time = line.strip().split(': ')
  
  for line in dice_scores:
    label, score = line.strip().split('\t')
    label = int(label)
    score = float(score)
    if label in scores:
      scores[label].append(score)
    else:
      scores[label] = [score]

  times.append(float(running_time))


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

  get_results_from_log(target_idx, global_scores, global_times)


def results_stats():
  import statistics

  OTHER_RESULTS = {
    2:  [0.93, 0.94],
    3:  [0.74, 0.72],
    4:  [0.84, 0.73],
    5:  [0.03, 0.0 ],
    7:  [0.67, 0.70],
    8:  [0.63, 0.61],
    10: [0.83, 0.83],
    14: [0.09, 0.0 ],
    15: [0.0,  0.20],
    16: [0.87, 0.89],
    17: [0.60, 0.62],
    24: [0.27, 0.10],
    41: [0.73, 0.78],
    42: [0.75, 0.73],
    43: [0.83, 0.78],
    46: [0.68, 0.59],
    47: [0.63, 0.60],
    49: [0.84, 0.81],
    53: [0.58, 0.50],
  }

  mean_running_time = statistics.mean(global_times)
  mean_scores = {}
  median_scores = {}
  min_scores = {}
  max_scores = {}
  for label, score_list in global_scores.items():
    if label < 2:
      continue;
    min_scores[label] = min(score_list)
    max_scores[label] = max(score_list)
    mean_scores[label] = statistics.mean(score_list)
    median_scores[label] = statistics.median(score_list)

  print('{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}{:<13}'.format(
          'Label', 'Min', 'Max', 'Mean', 'Median', 'vs. KeyPoints', 'vs. SyN')
       )
  for label in global_scores:
    if label < 2:
      continue
    if label in OTHER_RESULTS:
      vs_key_points = mean_scores[label] - OTHER_RESULTS[label][0]
      vs_syn = mean_scores[label] - OTHER_RESULTS[label][1]
    else:
      vs_key_points = 0.0
      vs_syn = 0.0

    print('{:<13}{:<13.5}{:<13.5}{:<13.5}{:<13.5}{:<13.5}{:<13.5}'.format(
            label,
            min_scores[label],
            max_scores[label],
            mean_scores[label],
            median_scores[label],
            vs_key_points,
            vs_syn)
         )

  print(mean_running_time)


def main():
  if not os.path.exists(OPAL_EXE):
    raise ValueError('Path to OPAL executable "{s}" is incorrect!'.format(s=OPAL_EXE))

  if not os.path.exists(IMAGES_BASE):
    raise ValueError('Path to images database "{s}" is incorrect!'.format(s=IMAGES_BASE))
  if not os.path.exists(SEGMENTATION_BASE):
    raise ValueError('Path to segmentation database "{s}" is incorrect!'.format(s=SEGMENTATION_BASE))

  images = sorted([f for f in os.listdir(IMAGES_BASE) if '.img' in f])
  segmentations = sorted([f for f in os.listdir(SEGMENTATION_BASE) if '.img' in f])
  
  if (len(images) != len(segmentations)):
    raise ValueError('Images count {i} != segmentations count {j}'.format(i=len(images), j=len(segmentations)))

  opal_settings = {
    'initWindowRadius': '10',
    'patchRadius': 12,
    'intermediateSaving': 'false',
    'intermediateSavingPath': '',
    'maxIterations': '30',
  }

  rmtree(RESULTS_ROOT_DIR, ignore_errors=True)
  os.mkdir(RESULTS_ROOT_DIR) 
  opal_config = os.path.join(RESULTS_ROOT_DIR, 'config.txt')
  make_opal_config(opal_config, **opal_settings)

  print('Running OPAL...')
  for i in range(len(images)):
    run_segmentation(opal_config, images, segmentations, i)
    print('Image {i} done'.format(i=i))

  results = {}
  results['times'] = global_times
  results['scores'] = global_scores
  with open('results.json', 'w') as f:
    json.dump(results, f, sort_keys=True, indent=4, separators=(',', ':'))

  results_stats()


if '__main__' == __name__:
  main()
