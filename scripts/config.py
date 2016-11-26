def make_opal_config(config_file, **kwargs):
  with open(config_file, 'w') as f:
    for name, value in kwargs.iteritems():
      f.write('{n}={v}\n'.format(n=name, v=value))
