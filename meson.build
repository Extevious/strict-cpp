project(
   'strict_types',
   'c',
   'cpp',
   default_options: ['c_std=c17', 'cpp_std=c++20'],
   license:         'MIT',
   license_files:   'LICENSE',
   meson_version:   '>=1.1.1',
   version:         '1.1.0',
)

strict_types_dep = declare_dependency(
   include_directories: include_directories('include'),
   version:             meson.project_version(),
)

strict_types_namespace          = get_option('namespace')
strict_types_enable_extra_types = get_option('enable_extra_types')
strict_types_build_examples     = get_option('build_examples')

if strict_types_enable_extra_types
   add_project_arguments('-DSTRICT_CPP_OPTIONAL_TYPES', language: 'cpp')
endif

if strict_types_namespace == ''
   error('the provided string for option "namespace" cannot be empty.')

elif strict_types_namespace.contains(' ')
   error('the provided string for option "namespace" cannot contain spaces.')

else
   add_project_arguments('-DSTRICT_CPP_NAMESPACE=' + strict_types_namespace, language: 'cpp')
endif

if strict_types_build_examples
   subdir('examples')
endif