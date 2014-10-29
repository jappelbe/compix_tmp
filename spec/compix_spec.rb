require 'spec_helper'

WHITE = 0xffffffff
ALMOST_WHITE = 0xfefefeff
BLACK = 0x000000ff

describe Compix do
  describe '#find_subimage' do
    let(:image) { OilyPNG::Canvas.new(3, 3, WHITE) }
    let(:threshold) { 0 }
    subject(:subimage_match) { Compix::find_subimage(subimage, image, threshold)}

    context "Subimage self of self" do
      let(:subimage) { image }
      it "finds subimage" do
        expected_match = Compix::SubimageMatch.new(0, 0, 9, 9)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Subimage of exact same color" do
      let(:subimage) { OilyPNG::Canvas.new(2, 2, WHITE) }
      it "finds a subimage of exact same color" do
        expected_match = Compix::SubimageMatch.new(0, 0, 4, 4)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Subimage not within threshold" do
      let(:subimage) { OilyPNG::Canvas.new(2, 2, ALMOST_WHITE) }
      it "does not find a subimage" do
        expected_match = Compix::SubimageMatch.new(-1, -1, 4, 0)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Subimage not within a large threshold" do
      let(:subimage) { OilyPNG::Canvas.new(2, 2, BLACK) }
      let(:threshold) { 250 }
      it "does not find a subimage" do
        expected_match = Compix::SubimageMatch.new(-1, -1, 4, 0)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Subimage is within threshold" do
      let(:subimage) { OilyPNG::Canvas.new(2, 2, ALMOST_WHITE) }
      let(:threshold) { 1 }
      it "finds a subimage" do
        expected_match = Compix::SubimageMatch.new(0, 0, 4, 4)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Subimage which is not in image" do
      let(:subimage) { OilyPNG::Canvas.new(2, 2, BLACK) }
      it "does not find a subimage" do
        expected_match = Compix::SubimageMatch.new(-1, -1, 4, 0)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Subimage is larger than source image" do
      let(:subimage) { OilyPNG::Canvas.new(4, 4, WHITE) }
      it "does not find a subimage" do
        expected_match = Compix::SubimageMatch.new(-1, -1, 16, 0)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Subimage in middle of image" do
      let(:subimage) { subimage = OilyPNG::Canvas.new(2,2, BLACK) }
      let(:pixels) { [WHITE, WHITE, WHITE, WHITE,
                      WHITE, BLACK, BLACK, WHITE,
                      WHITE, BLACK, BLACK, WHITE,
                      WHITE, WHITE, WHITE, WHITE ] }
      let(:image) { image = OilyPNG::Canvas.new(4, 4, pixels) }
      it "finds a subimage in correct coordinates" do
        expected_match = Compix::SubimageMatch.new(1, 1, 4, 4)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Subimage in down-right corner" do
      let(:subimage) { subimage = OilyPNG::Canvas.new(2,2, BLACK) }
      let(:pixels) { [WHITE, WHITE, WHITE, WHITE,
                      WHITE, WHITE, WHITE, WHITE,
                      WHITE, WHITE, BLACK, BLACK,
                      WHITE, WHITE, BLACK, BLACK ] }
      let(:image) { image = OilyPNG::Canvas.new(4, 4, pixels) }
      it "finds a subimage in correct coordinate" do
        expected_match = Compix::SubimageMatch.new(2, 2, 4, 4)
        expect(subimage_match).to eql(expected_match)
      end
    end
  end
end
