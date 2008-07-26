import distutils.sysconfig
print '-I%s' % distutils.sysconfig.get_python_inc()
