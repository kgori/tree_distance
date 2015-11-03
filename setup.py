try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension

from Cython.Distutils import build_ext
import pkg_resources
import platform, re, subprocess

def is_clang(bin):
    proc = subprocess.Popen([bin, '-v'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = proc.communicate()
    output = '\n'.join([stdout, stderr])
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

ext = Extension("tree_distance",
                language='c++',
                sources = ['BipartiteGraph.cpp',
                           'Bipartition.cpp',
                           'Distance.cpp',
                           'Geodesic.cpp',
                           'PhyloTree.cpp',
                           'PhyloTreeEdge.cpp',
                           'Ratio.cpp',
                           'RatioSequence.cpp',
                           'Tools.cpp',
                           'cython/tree_distance.pyx'],
                # include_dirs = [data_dir],
                extra_compile_args=['-std=c++11'],
               )

setup(cmdclass={'build_ext':my_build_ext},
      name="tree_distance",
      version="0.0.7",
      author='Kevin Gori',
      author_email='kgori@ebi.ac.uk',
      description='Wrapper for GTP tree distances in c++',
      url='https://bitbucket.org/kgori/cgtp.git',
      ext_modules = [ext],
      install_requires=[
        'cython',
      ],
     )
