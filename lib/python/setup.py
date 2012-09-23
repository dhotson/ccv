from distutils.core import setup, Extension

ccv = Extension('ccv',
    include_dirs = ['..', '/usr/X11/include'],
    libraries = ['m', 'jpeg', 'png', 'z', 'fftw3f', 'fftw3', 'linear'],
    library_dirs = ['..', '/usr/X11/lib'],
    extra_objects = ['../libccv.a'],
    sources = ['ccv_python.c']
)

setup (name = 'ccv',
    version = '0.1',
    description = 'CCV - a modern computer vision library',
    ext_modules = [ccv])

