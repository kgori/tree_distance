try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension

from Cython.Distutils import build_ext
import pkg_resources

# data_dir = pkg_resources.resource_filename("autowrap", "data_files")

ext = Extension("TreeDistance",
                language='c++',
                sources = ['BipartiteGraph.cpp',
                           'Bipartition.cpp',
                           'Distance.cpp',
                           'EdgeAttribute.cpp',
                           'Geodesic.cpp',
                           'PhyloTree.cpp',
                           'PhyloTreeEdge.cpp',
                           'Ratio.cpp',
                           'RatioSequence.cpp',
                           'Tools.cpp',
                           'cython/TreeDistance.pyx'],
                # include_dirs = [data_dir],
                extra_compile_args=['-std=c++11'],
               )

setup(cmdclass={'build_ext':build_ext},
      name="TreeDistance",
      version="0.0.1",
      author='Kevin Gori',
      author_email='kgori@ebi.ac.uk',
      description='Wrapper for GTP tree distances in c++',
      url='https://bitbucket.org/kgori/cgtp.git',
      ext_modules = [ext],
      install_requires=[
        'cython',
      ],
     )
