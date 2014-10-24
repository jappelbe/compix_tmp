Gem::Specification.new do |s|
  s.name    = "compix"
  s.version = "0.0.5"
  s.summary = "Compix for oilypng"
  s.author  = "Jon Appelberg"
  
  s.files = Dir.glob("ext/**/*.{c,h,rb}") +
            Dir.glob("lib/**/*.rb")
  
  s.extensions << "ext/compix/extconf.rb"
  
  s.add_development_dependency "rake-compiler"
end
