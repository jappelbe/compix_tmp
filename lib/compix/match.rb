require 'chunky_png'


class Compix::SubimageMatch
  attr_reader :coordinate_x, :coordinate_y, :pixels_compared, :pixels_matched

  def initialize(coord_x, coord_y, pixels_compared, pixels_matched)
    raise ArgumentError.new("Matched more pixels ('#{pixels_matched}') than compared ('#{pixels_compared}')") if pixels_compared < pixels_matched
    @coordinate_x = coord_x
    @coordinate_y = coord_y
    @pixels_compared = pixels_compared
    @pixels_matched = pixels_matched
  end

  def match_percentage
    return 0 if pixels_compared == 0
    pixels_matched.to_f / pixels_compared
  end

  def eql?(other)
    other.class.eql?(self.class) && other.state.eql?(self.state)
  end

  def state
    [@coordinate_x, @coordinate_y, @pixels_compared, @pixels_matched]
  end
end
