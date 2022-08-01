# gobjectrs.meson

This repository is used as an helper to build rust code which uses
[gtk-rs](https://gtk-rs.org/) using meson.

Libraries are built using [cargo-c](https://github.com/lu-zero/cargo-c).

You can use it by making this repository a submodule (or copy the files) and
then from the main meson file do:

``` meson
# Definition of the libraries/crates to be build as GObject Introspectable libraries
gobject_rust_libraries_defs = {
  'gobject-example': { # Workspace name of the library
    'dir': '.', # Directory where the library is implemented
    'libname': 'gobject_example',
    'dependencies':         [
      dependency('gobject-2.0'),
      cc.find_library('m', required : false),
    ],
    # List of headers that will be:
    # - installed in '${includedir}/${Workspace name of the library}'
    # - be used to generate gobject-introspection files
    'headers': [
      'ex.h',
    ],
    # Uses [gnome.generate_gir](https://mesonbuild.com/Gnome-module.html#gnomegenerate_gir)
    # so you can pass all the options from this function in the following dictionary
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

// Setup rust tests
test('rs-tests',
  grs_cargo_wrapper,
  args: ['test'] + grs_cargo_build_defaults,
  timeout: 600)
```
