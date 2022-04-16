project('gobject-example-rs',
        'rust',
        'c',
        version: '0.1',
        meson_version : '>= 0.60')


cc = meson.get_compiler('c')

# Definition of the libraries/crates to be build as GObject Introspectable librairies
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

### Geneic implementation to build Rust libraries exposing GObjects {
build_type = 'release'
if get_option('debug')
  build_type = 'debug'
endif

version = meson.project_version()
cargo = find_program('cargo', version:'>=1.40')
cargo_wrapper = find_program('scripts/cargo_wrapper.py')
cargo_c = find_program('cargo-cbuild', version:'>=0.9.3', required: false)
rustc = find_program('rustc', version:'>=1.52')
build_vala = add_languages('vala', required: get_option('vala'))

if not cargo_c.found()
  error('cargo-c missing, install it with: \'cargo install cargo-c\'')
endif

fs = import('fs')
gnome = import('gnome')
python = import('python').find_installation()
pkgconfig = import('pkgconfig')

system = build_machine.system()
extension = 'a'
if system == 'windows'
  extension = 'lib'
endif

cargo_build_defaults = [
  '--build-dir', meson.current_build_dir(),
  '--source-dir', meson.current_source_dir(),
  '--include-directories', gobject_rust_libraries_defs.keys(),
  '--root-dir', meson.project_build_root(),
  '--build-type', build_type,
  '--prefix', get_option('prefix'),
  '--libdir', get_option('libdir'),
  '--version', version,
  '--local-include-dir', 'include',
  '--depfile', '@DEPFILE@',
  '--exts', extension
]

# Extra environment variables to pass when running cargo
foreach p, lib_info : gobject_rust_libraries_defs
  libname = lib_info.get('libname')

  # We checkout generated headers inside the git repository to better handle/see
  # how they are generated/modified.
  pkg_name = p + '-' + version
  include_dir = 'include' / p
  headers = []
  gir_extras = []
  foreach header:  lib_info.get('headers')
    headers += [include_dir / header]
    gir_extras += ['--c-include=' + header]
    install_headers(include_dir / header, install_dir: pkg_name)
  endforeach

  # Use all files checked in git to  figure out when to rebuild
  deps = lib_info.get('dependencies', [])

  # Depends on our dependencies so it can be used as subproject
  depends = []
  foreach dep: deps
    if dep.type_name() == 'internal'
      depends += [dep]
    endif
  endforeach

  # Build the static library with cargo
  cargo_dep = declare_dependency(
    link_whole: custom_target(libname + '.tmp',
      output: ['lib' + libname + '.' + extension],
      depends: depends,
      install: get_option('default_library') != 'shared',
      console: true,
      depfile: libname + '.dep',
      command: [cargo_wrapper,
        'build',
        '--extra-env-vars', lib_info.get('enviroment_variables', []),
      ] + cargo_build_defaults
    ),
  )

  # We build an actual shared library so we can build the gir from it
  lib = build_target(
    libname,
    target_type: 'library',
    version: version,
    include_directories: include_directories(include_dir),
    install: true,
    dependencies: [
      cargo_dep,
      deps,
    ],
  )

  sources = []
  if lib_info.has_key('gir')
    gir_def = lib_info.get('gir')
    gir = gnome.generate_gir(lib, kwargs:
      gir_def + {
        'sources': gir_def.get('sources', []) + headers,
        'extra_args': gir_extras,
      }
    )
    sources += [gir]
  endif

  vala_deps = []
  if build_vala
      vala_deps = [gnome.generate_vapi(pkg_name, sources: gir.get(0), install: true)]
  endif

  pkgconfig.generate(lib,
    libraries : [lib],
    name: pkg_name,
    subdirs: [
      pkg_name
    ]
  )

  # Make the include dir as it needs to exist to be referenceable
  set_variable(p.replace('-', '_') + '_dep',
    declare_dependency(
      link_with: lib,
      sources: sources,
      dependencies: deps,
      include_directories: include_dir,
    )
  )

  if vala_deps.length() > 0
    # We need to make another dep for vala consumption
    # otherwise we get 'ERROR: Vala library 'c_test' has no Vala or Genie source files.'
    set_variable(p.replace('-', '_') + '_vala_dep',
      declare_dependency(
        dependencies: [get_variable(p.replace('-', '_') + '_dep'), vala_deps],
      )
    )
  endif

endforeach
# }

subdir('tests')
