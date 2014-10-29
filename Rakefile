require 'rake/extensiontask'
require 'rspec/core/rake_task'


spec = Gem::Specification.load('compix.gemspec')
Rake::ExtensionTask.new('compix') do |ext|
  #ext.lib_dir = File.join('lib', 'compix')
  ext.config_options = '--with-cflags="-std=c99 -Wall -pedantic -g"'
  ext.gem_spec = spec
end

RSpec::Core::RakeTask.new(:spec)

Rake::Task['spec'].prerequisites << :compile