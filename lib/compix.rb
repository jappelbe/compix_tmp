require 'compix.so'
require 'compix/match.rb'

module Compix
  def self.find_all_instances_of_subimage(subimage, image, options={})
    options[:start_coordinate_x] ||= 0
    options[:start_coordinate_y] ||= 0
    matches = []

    subimage_match = self.find_subimage(subimage, image, options)
    while subimage_match.match?
      skip = false
      matches.each do |match|
        if match.overlaped_by?(subimage_match)
          skip = true
          break
        end
      end
      matches << subimage_match unless skip
      options[:start_coordinate_y] = subimage_match.coordinate_y
      options[:start_coordinate_x] = subimage_match.coordinate_x + subimage.width
      subimage_match = self.find_subimage(subimage, image, options)
    end
    matches
  end
end