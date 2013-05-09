#!/usr/bin/env python

subdirs = ['src', 'example']

def options(opt):
  opt.load('compiler_cxx')
  opt.load('gnu_dirs')

  opt.add_option('--disable-c-binding',
                 dest='c_binding',
                 action='store_false',
                 default=True,
                 help='disable C binding')

  opt.recurse(subdirs)

def configure(conf):
  conf.load('compiler_cxx')
  conf.load('gnu_dirs')

  conf.env.CXXFLAGS += ['-O2', '-Wall', '-g', '-pipe']

  conf.check_cfg(package = 'jubatus', args = '--cflags --libs')
  conf.check_cxx(lib = 'jubastorage')
  conf.check_cxx(lib = 'jubaconverter')
  conf.check_cxx(lib = 'jubatus_driver')
  conf.check_cfg(package = 'pficommon', args = '--cflags --libs')

  conf.env.C_BINDING = conf.options.c_binding

  conf.recurse(subdirs)

def build(bld):
  bld.recurse(subdirs)
