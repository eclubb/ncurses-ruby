require 'rake/gempackagetask'

SUMMARY  = 'This wrapper provides access to the functions, macros, global variables and constants ' +
           'of the ncurses library.  These are mapped to a Ruby Module named "Ncurses":  ' +
           'Functions and external variables are implemented as singleton functions of the Module Ncurses.'

spec = Gem::Specification.new do |s|
  s.name          = 'ncurses'
  s.email         = 't-peters@users.berlios.de'
  s.author        = 'Tobias Peters'
  s.version       = '1.1'
  s.summary       = SUMMARY
  s.platform      = Gem::Platform::RUBY
  s.has_rdoc      = false
  s.homepage      = 'http://ncurses-ruby.berlios.de'
  s.description   = SUMMARY
  s.require_paths = ["lib"]
  s.files         = Dir.glob("[A-Z]*") + Dir.glob("*.{c,h,rb}") + Dir.glob("{examples,lib}/**/*")
  s.extensions    = FileList["ext/**/extconf.rb"]
end

# add your default gem packing task
Rake::GemPackageTask.new(spec) do |pkg|
end

begin
  require 'rake/extensiontask'
rescue LoadError
  warn "To compile native gems, install rake-compiler (gem install rake-compiler)"
else
  Rake::ExtensionTask.new('ncurses', spec)
end
