# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'ncurses-ruby/version'

DESCRIPTION = 'This wrapper provides access to the functions, macros, global ' +
              'variables and constants of the ncurses library. These are ' +
              'mapped to a Ruby Module named "Ncurses". Functions and ' +
              'external variables are implemented as singleton functions of ' +
              'the Ncurses module.'

Gem::Specification.new do |spec|
  spec.name          = 'ncurses-ruby'
  spec.version       = Ncurses::Ruby::VERSION
  spec.platform      = Gem::Platform::RUBY
  spec.authors       = ['Tobias Herzke', 'Simon Kaczor', 'Earle Clubb']
  spec.email         = ['t-peters@users.berlios.de', 'skaczor@cox.net', 'eclubb@valcom.com']
  spec.has_rdoc      = false
  spec.homepage      = 'http://github.com/eclubb/ncurses-ruby'
  spec.summary       = 'A Ruby module for accessing the ncurses library'
  spec.description   = DESCRIPTION
  spec.license       = "LGPL"

  spec.require_paths = ['lib']
  spec.files         = `git ls-files -z`.split("\x0")
  spec.extensions    = ['ext/ncurses/extconf.rb']
end
