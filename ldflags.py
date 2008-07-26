import distutils.sysconfig
print distutils.sysconfig.get_config_var('LINKFORSHARED')
