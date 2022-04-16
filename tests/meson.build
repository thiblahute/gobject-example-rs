test('rs-tests',
  cargo_wrapper,
  args: ['test'] + cargo_build_defaults,
  timeout: 600)

test('c',
  executable('c_test', 'test.c', dependencies: gobject_example_dep)
)

if build_vala
  test('vala',
    executable('vala_test', 'test.vala', dependencies: gobject_example_vala_dep)
  )
endif

test_env = environment()
test_env.set('GI_TYPELIB_PATH', meson.current_build_dir() / '..')

test('python', python, args: [files('test.py')], env: test_env)

gjs = find_program('gjs', required: get_option('js'))
if gjs.found()
  test('javascript', gjs, args: [files('test.js')], env: test_env)
else
  warning('`gjs` not found, not testing JS binding.')
endif
