from distutils.core import setup, Extension
module1 = Extension('filelog',
        extra_compile_args=['-std=c++11'],
        sources = ['./filelog.cpp', './pyFilelog.cpp'])
setup (name = 'fileLog',
        version = '1.0',
        description = 'multiprocess filelog',
        ext_modules = [module1])

