from setuptools import setup, Extension

from Cython.Distutils import build_ext
import pkg_resources
import os, platform, re, subprocess

def is_clang(bin):
    proc = subprocess.Popen([bin, '-v'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = proc.communicate()
    output = str(b'\n'.join([stdout, stderr]).decode('ascii', 'ignore'))
    return not re.search(r'clang', output) is None

class my_build_ext(build_ext):
    def build_extensions(self):
        binary = self.compiler.compiler[0]
        if is_clang(binary):
            for e in self.extensions:
                e.extra_compile_args.append('-stdlib=libc++')
                if platform.system() == 'Darwin':
                    e.extra_compile_args.append('-mmacosx-version-min=10.7')
                    e.extra_link_args.append('-mmacosx-version-min=10.7')

        build_ext.build_extensions(self)

"""
Removed autowrap dependency, so no need for this. Leaving it here in case it needs reinstating in the future.
try:
    data_dir = pkg_resources.resource_filename("autowrap", os.path.join("data_files", "autowrap"))
except ImportError:
    from setuptools.dist import Distribution
    Distribution().fetch_build_eggs(['autowrap'])
    data_dir = pkg_resources.resource_filename("autowrap", os.path.join("data_files", "autowrap"))
"""

ext = Extension("tree_distance",
                language='c++',
                sources = ['src/BipartiteGraph.cpp',
                           'src/Bipartition.cpp',
                           'src/Distance.cpp',
                           'src/Geodesic.cpp',
                           'src/PhyloTree.cpp',
                           'src/PhyloTreeEdge.cpp',
                           'src/Ratio.cpp',
                           'src/RatioSequence.cpp',
                           'src/Tools.cpp',
                           'cython/tree_distance.pyx'],
                include_dirs = ['src/include'], # removed data_dir
                extra_compile_args=['-std=c++11'],
               )

setup(cmdclass={'build_ext':my_build_ext},
      name="tree_distance",
      version="1.0.6",
      author='Kevin Gori',
      author_email='kgori@ebi.ac.uk',
      description='Wrapper for GTP tree distances in c++',
      url='https://github.com/kgori/tree_distance.git',
      ext_modules = [ext],
      install_requires=[
          'cython',
      ],
     )
