require 'chunky_png'

DEFAULT_PERCENT = 0.85

class Compix::SubimageMatch
  attr_reader :coordinate_x, :coordinate_y, :pixels_compared, :pixels_matched, :width, :height

  def initialize(coord_x, coord_y, pixels_compared, pixels_matched, width, height, limit_for_match=DEFAULT_PERCENT)
    raise ArgumentError.new("Matched more pixels ('#{pixels_matched}') than compared ('#{pixels_compared}')") if pixels_compared < pixels_matched
    @coordinate_x = coord_x
    @coordinate_y = coord_y
    @pixels_compared = pixels_compared
    @pixels_matched = pixels_matched
    @width = width
    @height = height
    @match = limit_for_match <= self.match_percentage
  end

  def match_percentage
    return 0 if pixels_compared == 0
    pixels_matched.to_f / pixels_compared
  end

  def match?
    @match
  end

  def eql?(other)
    other.class.eql?(self.class) && other.state.eql?(self.state)
  end

  def overlaped_by?(other)
    return false if other.coordinate_x + other.width <= @coordinate_x
    return false if other.coordinate_x >= @coordinate_x + @width
    return false if other.coordinate_y + other.height <= @coordinate_y
    return false if other.coordinate_y >= @coordinate_y + @height
    true
  end

  def state
    [@coordinate_x, @coordinate_y, @pixels_compared, @pixels_matched]
  end
end
