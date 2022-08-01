project('gobject-example-rs',
        'rust',
        'c',
        version: '0.1',
        meson_version : '>= 0.60')



grs_build_vala = add_languages('vala', required: get_option('vala'))

cc = meson.get_compiler('c')

# Definition of the libraries/crates to be build as GObject Introspectable libraries
gobject_rust_libraries_defs = {
  'gobject-example': { # Workspace name of the library
    'dir': '.', # Directory where the library is implemented
    'libname': 'gobject_example',
    'dependencies': [
      dependency('gobject-2.0'),
      cc.find_library('m', required : false),
    ],
    'headers': [
      'ex.h',
    ],
    'gir': {
      'namespace' : 'Ex',
      'nsversion' : '0.1',
      'export_packages' : 'ex',
      'includes' : ['GLib-2.0', 'GObject-2.0' ],
      'install' : true,
    },
  }
}

subdir('gobjectrs.meson')
subdir('tests')
