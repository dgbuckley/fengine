project(
	'thecalm',
	'c',
	version: '0.0.0',
	license: 'MPLv2',
	meson_version: '>=0.47.0',
	default_options: [
		'c_std=c99',
		'warning_level=3',
		'werror=true',
	],
)

cc = meson.get_compiler('c')

add_project_arguments(cc.get_supported_arguments([
		'-Wno-missing-braces',
		'-Wno-unused-parameter',
		'-Wno-unused-variable',
		'-Wno-pointer-arith',
		'-Wno-unused-but-set-variable',
		'-Wno-sign-compare',
]), language: 'c')

engine_inc = include_directories('include')

decl_exe = executable(
	'fengine',
	files(
		'src/main.c',
		'src/buf.c',
	),
	include_directories: engine_inc,
	dependencies: [ ],
	install: true,
)
