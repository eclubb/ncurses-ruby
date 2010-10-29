# -*- encoding: utf-8 -*-

$:.push File.expand_path('../lib', __FILE__)
require 'rubygems'
require 'ncurses-ruby/version'

SUMMARY = 'This wrapper provides access to the functions, macros, global variables and constants ' +
           'of the ncurses library.  These are mapped to a Ruby Module named "Ncurses":  ' +
           'Functions and external variables are implemented as singleton functions of the Module Ncurses.'

spec = Gem::Specification.new do |s|
  s.name        = 'ncurses-ruby'
  s.version     = Ncurses::Ruby::VERSION
  s.platform    = Gem::Platform::RUBY
  s.authors     = ['Tobias Herzke', 'Simon Kaczor', 'Earle Clubb']
  s.email       = ['t-peters@users.berlios.de', 'skaczor@cox.net', 'eclubb@valcom.com']
  s.has_rdoc    = false
  s.homepage    = 'http://github.com/eclubb/ncurses-ruby'
  s.summary     = SUMMARY
  s.description = SUMMARY

  s.require_paths = ['lib']
  s.files         = Dir.glob("[A-Z]*") + Dir.glob("{examples,lib,ext}/**/*")
  s.extensions    = ['ext/ncurses/extconf.rb']
end

if $0 == __FILE__
  Gem::Builder.new(spec).build
end
