__id__       = "$Id: setup.py 239 2008-01-07 17:47:47Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 239 $"
__date__     = "$Date: 2008-01-07 10:47:47 -0700 (Mon, 07 Jan 2008) $"

import distutils.ccompiler
import distutils.sysconfig
from distutils.core import setup, Extension
from distutils.command.build import build
import os
import numpy

class modified_compiler(distutils.unixccompiler.UnixCCompiler):
    pass

class modified_build(distutils.command.build.build):

    sub_commands = [('build_clib',    build.has_c_libraries),
                    ('build_ext',     build.has_ext_modules),
                    ('build_py',      build.has_pure_modules),
                    ('build_scripts', build.has_scripts),
                    ]


# for key in distutils.sysconfig.get_config_vars().keys():
#     print "%s, %s" %(key, distutils.sysconfig.get_config_vars(key))

#---Definitions for all modules---
define_macros = [('NITERCHK', '1'), ('NOASSERT', '1')]
undef_macros = []
extra_compile_args = ['-funroll-loops']
pyModules = []
cppModules = []
packages = ['Transport']
packagedir = {'Transport':''}

# Obtain the numpy include directory.  This logic works across numpy versions.
try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()

include_dirs = ['.', 'Material', 'Misc', 'Geometry/OneDMesh', \
    'Geometry/OneDMesh/Iterators', 'Geometry/Misc', 'Exception', \
    'Geometry/Field', 'FissionSource', 'Particle/', 'test',\
    os.path.expanduser('~/src/boost_1_34_1/')]
include_dirs.append(numpy_include)

#---Modules definitions---
#---Markov module---
# MarkFiles = ['Markov.i']
# MarkSource = [os.path.join('Markov', file) for file in MarkFiles]
# Markov= Extension('Transport.Markov._Markov',
#          sources = MarkSource, include_dirs=include_dirs)
# cppModules.append(Markov)
# pyModules.append('Transport.Markov.Markov')
# pyModules.append('Transport.Markov.Power')

#---PowerBase module---
PowerBaseFiles = ['PowerBase.i']
PowerBaseSource = [os.path.join('Markov', file) for file in PowerBaseFiles]
PowerBase = Extension('Transport.Markov._PowerBase',
         sources = PowerBaseSource, include_dirs=include_dirs,
         language = "c++", define_macros = define_macros, undef_macros =
         undef_macros, extra_compile_args = extra_compile_args)
cppModules.append(PowerBase)
pyModules.append('Transport.Markov.PowerBase')

pyModules.append('Transport.Markov.Power')


#---Material module---
MatFiles = ['Material.i', 'Material.cpp']
MatSource = [os.path.join('Material', file) for file in MatFiles]
Material= Extension('Transport.Material._Material',
         sources = MatSource, include_dirs=include_dirs,
         language = "c++", define_macros = define_macros, undef_macros =
         undef_macros, extra_compile_args = extra_compile_args)
pyModules.append('Transport.Material.Material')
cppModules.append(Material)

#---FissionSource package---
packages.append('Transport.FissionSource')

#---BankSource sub-module---
BankFiles = ['BankSource.i', 'BankSource.cpp', 'FissionSource.cpp', \
    '../RandomLib/Random.cpp', '../Particle/Particle.cpp', \
    '../Geometry/OneDMesh/OneDCartMesh.cc', \
    '../Geometry/OneDMesh/OneDZone.cc', \
    '../Geometry/OneDMesh/OneDNode.cc', ]

BSource = [os.path.join('FissionSource', file) for file in BankFiles]
BankSource = Extension('Transport.FissionSource._BankSource',
         sources = BSource, include_dirs=include_dirs,
         language = "c++", define_macros = define_macros, undef_macros =
         undef_macros, extra_compile_args = extra_compile_args)
pyModules.append('Transport.FissionSource.BankSource')
cppModules.append(BankSource)

#---HistSource sub-module---
HistFiles = ['HistSource.i', 'HistSource.cpp', 'FissionSource.cpp', \
    '../RandomLib/Random.cpp', '../Particle/Particle.cpp', \
    '../Geometry/OneDMesh/OneDCartMesh.cc', \
    '../Geometry/OneDMesh/OneDZone.cc', \
    '../Geometry/OneDMesh/OneDNode.cc', ]

HSource = [os.path.join('FissionSource', file) for file in HistFiles]
HistSource = Extension('Transport.FissionSource._HistSource',
         sources = HSource, include_dirs=include_dirs,
         language = "c++", define_macros = define_macros, undef_macros =
         undef_macros, extra_compile_args = extra_compile_args)
pyModules.append('Transport.FissionSource.HistSource')
cppModules.append(HistSource)


#---Geometry package---
packages.append('Transport.Geometry')

#---Mesh sub-module---
MeshFiles = ['OneDCartMesh.i', 'OneDCartMesh.cc', 'OneDZone.cc', 'OneDNode.cc']
MeshSource = [os.path.join('Geometry/OneDMesh', file) for file in MeshFiles]
Mesh = Extension('Transport.Geometry.OneDMesh._Mesh', sources = MeshSource,
       include_dirs=include_dirs,
       language = "c++", define_macros = define_macros, undef_macros =
       undef_macros, extra_compile_args = extra_compile_args)
pyModules.append('Transport.Geometry.OneDMesh.Mesh')
cppModules.append(Mesh)

#---Field sub-module---
FieldFiles = ['Field.i']
FieldSource = [os.path.join('Geometry/Field', file) for file in FieldFiles]
Field = Extension('Transport.Geometry.Field._Field', sources = FieldSource,
       include_dirs=include_dirs,
       language = "c++", define_macros = define_macros, undef_macros =
       undef_macros, extra_compile_args = extra_compile_args)
pyModules.append('Transport.Geometry.Field.Field')
cppModules.append(Field)

#---SWIG_test module---
TestFiles = ['test_SWIG.i', 'test_SWIG.cpp']
TestSource = [os.path.join('test', file) for file in TestFiles]
Test = Extension('Transport.test._test_SWIG', sources = TestSource,
        include_dirs = include_dirs,
        language = "c++", define_macros = define_macros, undef_macros =
        undef_macros, extra_compile_args = extra_compile_args)
pyModules.append('Transport.test.test_SWIG')
cppModules.append(Test)

packages.append('Transport.test')


setup(name = 'Transport',
       version = '0.5',
       description = 'Monte Carlo Particle Transport Options',
       author = 'Jeremy Conlin',
       author_email = 'jlconlin@umich.edu',
       url = 'http://',
       package_dir = packagedir,
       packages = packages,
       ext_modules = cppModules,
       py_modules = pyModules,
       cmdclass = {"build": modified_build}
       )

