project(
   'strict-cpp',
   'c',
   'cpp',
   default_options: ['c_std=c17', 'cpp_std=c++20'],
   license: 'MIT',
   license_files: 'LICENSE',
   meson_version: '>=1.1.1',
   version: '1.0.0',
)

strict_cpp_dep = declare_dependency(
   include_directories: include_directories('include'),
   version: meson.project_version(),
)

strict_cpp_namespace = get_option('namespace')
strict_cpp_enable_optional_types = get_option('enable_optional_types')
strict_cpp_build_examples = get_option('build_examples')

if strict_cpp_enable_optional_types
   add_project_arguments('-DSTRICT_CPP_OPTIONAL_TYPES', language: 'cpp')
endif

if strict_cpp_namespace == ''
   error('the provided string for option "namespace" cannot be empty.')

elif strict_cpp_namespace.contains(' ')
   error('the provided string for option "namespace" cannot contain spaces.')

else
   add_project_arguments('-DSTRICT_CPP_NAMESPACE=' + strict_cpp_namespace, language: 'cpp')
endif

if strict_cpp_build_examples
   subdir('examples')
endif