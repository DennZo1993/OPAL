def make_opal_config(config_file, **kwargs):
  with open(config_file, 'w') as f:
    for name, value in kwargs.items():
      f.write('{n}={v}\n'.format(n=name, v=value))


def make_default_opal_config(config_file):
  opal_settings = {
    'initWindowRadius': '5',
    'patchRadius': '10',
    'intermediateSaving': 'false',
    'intermediateSavingPath': '',
    'maxIterations': '50',
  }

  make_opal_config(config_file, **opal_settings) 
