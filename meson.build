project('io.github.potatowizard23.unwrap', 'c', version : '0.1.0')

gnome = import('gnome')

resources = gnome.compile_resources(
  'gresource', 'res/io.github.potatowizard23.unwrap.gresource.xml',
  source_dir: 'res',
  export: true,
)

configure_file(input : 'src/config.h.in',
  output : 'config.h',
  configuration : {
    'version' : meson.project_version(),
    'app-id' : meson.project_name(),
  }
)

adw = dependency('libadwaita-1')

executable(
  meson.project_name(),
  'src/main.c', 'src/app.c',
  resources, dependencies : adw
)
