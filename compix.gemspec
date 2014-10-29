Gem::Specification.new do |s|
  s.name    = "compix"
  s.version = "0.2.0"
  s.summary = "Compix for oilypng"
  s.homepage = "https://github.com/jappelbe/compix_tmp"
  s.author  = "Jon Appelberg"
  
  s.files = Dir.glob("ext/**/*.{c,h,rb}") +
            Dir.glob("lib/**/*.rb")

  s.extensions << "ext/compix/extconf.rb"
  
  s.add_development_dependency "rake-compiler"
  s.add_development_dependency "rake"
  s.add_development_dependency "rspec"
  s.add_runtime_dependency('oily_png')
end
