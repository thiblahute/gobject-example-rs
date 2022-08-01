### Geneic implementation to build Rust libraries exposing GObjects
# Variables to set before calling:
# - gobject_rust_libraries_defs:

grs_rel_base_dir = get_variable('grs_rel_base_dir', '..')

grs_build_type = 'release'
if get_option('debug')
  grs_build_type = 'debug'
endif

grs_version = meson.project_version()
grs_cargo = find_program('cargo', version:'>=1.40')
grs_cargo_wrapper = find_program('scripts/cargo_wrapper.py')
grs_cargo_c = find_program('cargo-cbuild', version:'>=0.9.3', required: false)
find_program('rustc', version:'>=1.52')

if not grs_cargo_c.found()
  error('cargo-c missing, install it with: \'cargo install cargo-c\'')
endif

grs_test_env = environment()
grs_test_env.set('GI_TYPELIB_PATH', meson.current_build_dir())
if build_machine.system() == 'windows'
    grs_test_env.append('PATH', meson.current_build_dir())
elif build_machine.system() == 'linux'
    grs_test_env.append('LD_LIBRARY_PATH', meson.current_build_dir())
else
    grs_test_env.append('DYLD_LIBRARY_PATH', meson.current_build_dir())
endif

grs_gnome = import('gnome')
grs_pkgconfig = import('pkgconfig')

system = build_machine.system()
extension = 'a'
if system == 'windows'
  extension = 'lib'
endif

grs_cargo_build_defaults = [
  '--build-dir', meson.current_build_dir(),
  '--source-dir', meson.current_source_dir() / grs_rel_base_dir,
  '--include-directories', gobject_rust_libraries_defs.keys(),
  '--root-dir', meson.project_build_root(),
  '--build-type', grs_build_type,
  '--prefix', get_option('prefix'),
  '--libdir', get_option('libdir'),
  '--version', grs_version,
  '--local-include-dir', 'include',
  '--depfile', '@DEPFILE@',
  '--exts', extension
]

# Extra environment variables to pass when running cargo
foreach grs_p, grs_lib_info : gobject_rust_libraries_defs
  grs_libname = grs_lib_info.get('libname')

  # We checkout generated headers inside the git repository to better handle/see
  # how they are generated/modified.
  grs_pkg_name = grs_p + '-' + grs_version
  include_dir = grs_rel_base_dir / 'include' / grs_p
  headers = []
  gir_extras = []
  foreach header:  grs_lib_info.get('headers')
    headers += [include_dir / header]
    gir_extras += ['--c-include=' + header]
    install_headers(include_dir / header, install_dir: grs_pkg_name)
  endforeach

  # Use all files checked in git to  figure out when to rebuild
  deps = grs_lib_info.get('dependencies', [])

  # Depends on our dependencies so it can be used as subproject
  depends = []
  foreach dep: deps
    if dep.type_name() == 'internal'
      depends += [dep]
    endif
  endforeach

  # Build the static library with cargo
  cargo_dep = declare_dependency(
    link_whole: custom_target(grs_libname + '.tmp',
      output: ['lib' + grs_libname + '.' + extension],
      depends: depends,
      install: get_option('default_library') != 'shared',
      console: true,
      depfile: grs_libname + '.dep',
      command: [grs_cargo_wrapper,
        'build',
        '--extra-env-vars', grs_lib_info.get('enviroment_variables', []),
      ] + grs_cargo_build_defaults
    ),
  )

  # We build an actual shared library so we can build the gir from it
  lib = build_target(
    grs_libname,
    target_type: 'library',
    version: grs_version,
    include_directories: include_directories(include_dir),
    install: true,
    dependencies: [
      cargo_dep,
      deps,
    ],
  )

  grs_sources = []
  if grs_lib_info.has_key('gir')
    grs_gir_def = grs_lib_info.get('gir')
    grs_gir = grs_gnome.generate_gir(lib, kwargs:
      grs_gir_def + {
        'sources': grs_gir_def.get('sources', []) + headers,
        'extra_args': gir_extras,
      }
    )
    grs_sources += [grs_gir]
  endif

  vala_deps = []
  if grs_build_vala
      vala_deps = [grs_gnome.generate_vapi(grs_pkg_name, sources: grs_gir.get(0), install: true)]
  endif

  grs_pkgconfig.generate(lib,
    libraries : [lib],
    name: grs_pkg_name,
    subdirs: [
      grs_pkg_name
    ]
  )

  # Make the include dir as it needs to exist to be referenceable
  set_variable(grs_p.replace('-', '_') + '_dep',
    declare_dependency(
      link_with: lib,
      sources: grs_sources,
      dependencies: deps,
      include_directories: include_dir,
    )
  )

  if vala_deps.length() > 0
    # We need to make another dep for vala consumption
    # otherwise we get 'ERROR: Vala library 'c_test' has no Vala or Genie source files.'
    set_variable(grs_p.replace('-', '_') + '_vala_dep',
      declare_dependency(
        dependencies: [get_variable(grs_p.replace('-', '_') + '_dep'), vala_deps],
      )
    )
  endif

endforeach
