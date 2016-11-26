from Common import *
from config import *
import subprocess
from shutil import copyfile


global_times = []
global_scores = {}


def run_segmentation(config, images, segmentations, target_idx, source_idx):
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
          os.path.join(SEGMENTATION_BASE, segmentations[target_idx]),
          os.path.join(IMAGES_BASE, images[source_idx]),
          os.path.join(SEGMENTATION_BASE, segmentations[source_idx])
         ]

  current_results_dir = os.path.join(RESULTS_ROOT_DIR, str(target_idx), 'with_one')
  if not os.path.exists(current_results_dir):
    os.makedirs(current_results_dir)

  args.append(current_results_dir)
  
  current_log_file = os.path.join(current_results_dir, 'log_{i}.txt'.format(i=source_idx))
  with open(current_log_file, 'w') as log:
    subprocess.call(args, stdout=log)

  copyfile(os.path.join(current_results_dir, 'result.png'), os.path.join(current_results_dir, 'result_{i}.png'.format(i=source_idx)))
  get_results_from_log(current_log_file, global_scores, global_times)


def main():
  images = get_images()
  segmentations = get_segmentations()

  if (len(images) != len(segmentations)):
    raise ValueError('Images count {i} != segmentations count {j}'.format(i=len(images), j=len(segmentations)))

  opal_config = os.path.join(RESULTS_ROOT_DIR, 'config.txt')
  make_default_opal_config(opal_config)

  target_idx = 2
  print('Running OPAL for {t}...'.format(t=target_idx))
  for i in range(len(images)):
    if i == target_idx:
      continue
    run_segmentation(opal_config, images, segmentations, target_idx, i)
  results_stats(global_times, global_scores)


if '__main__' == __name__:
  main()
