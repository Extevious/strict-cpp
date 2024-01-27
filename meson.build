project(
    'strict-cpp',
    'c',
    'cpp', 
    default_options : ['c_std=c17', 'cpp_std=c++20'],
    license         : 'MIT',
    license_files   : 'LICENSE',
    meson_version   : '>=1.1.1',
    version         : '1.0.0'
)

strict_cpp_dep = declare_dependency(
   include_directories : include_directories('include'),
   dependencies        : [],
   version             : meson.project_version()
)

if get_option('examples')
   subdir('examples')
endif