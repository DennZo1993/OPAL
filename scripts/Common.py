import re
import os


OPAL_EXE = '../build/main/OPAL'
IMAGES_BASE = '../test/test_data/Images'
SEGMENTATION_BASE = '../test/test_data/Segmentations'
RESULTS_ROOT_DIR = 'results'


def get_images():
  if not os.path.exists(IMAGES_BASE):
    raise ValueError('Path to images database "{s}" is incorrect!'.format(s=IMAGES_BASE))
  return sorted([f for f in os.listdir(IMAGES_BASE) if '.img' in f])


def get_segmentations():
  if not os.path.exists(SEGMENTATION_BASE):
    raise ValueError('Path to segmentation database "{s}" is incorrect!'.format(s=SEGMENTATION_BASE))
  return sorted([f for f in os.listdir(SEGMENTATION_BASE) if '.img' in f])


def get_results_from_log(log_file, scores, times):
  dice_scores = []
  running_time = ''

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


def results_stats(global_times, global_scores):
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
  for label in sorted(global_scores.keys()):
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
