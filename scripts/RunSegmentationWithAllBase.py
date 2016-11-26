import os
import subprocess
from shutil import rmtree

from config import make_opal_config


OPAL_EXE = '../build/main/OPAL'
IMAGES_BASE = '../test/test_data/Images'
SEGMENTATION_BASE = '../test/test_data/Segmentations'
RESULTS_ROOT_DIR = 'results'


def run_segmentation(config, images, segmentations, target_idx):
  '''
  OPAL <config_file>
       <target_image> <ground_truth_seg>
       <base_image_1> <base_seg_1>
       ...
       <base_image_n> <base_seg_n>
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

  for i in range(len(images)):
    run_segmentation(opal_config, images, segmentations, i)

if '__main__' == __name__:
  main()
