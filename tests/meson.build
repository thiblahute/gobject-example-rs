test('rs-tests',
  grs_cargo_wrapper,
  args: ['test'] + grs_cargo_build_defaults,
  timeout: 600)

test('c',
  executable('c_test', 'test.c', dependencies: gobject_example_dep)
)

if grs_build_vala
  test('vala',
    executable('vala_test', 'test.vala', dependencies: gobject_example_vala_dep)
  )
endif

python = import('python').find_installation()
test('python', python, args: [files('test.py')], env: grs_test_env)

gjs = find_program('gjs', required: get_option('js'))
if gjs.found()
  test('javascript', gjs, args: [files('test.js')], env: grs_test_env)
else
  warning('`gjs` not found, not testing JS binding.')
endif
