require 'spec_helper'

WHITE = 0xffffffff
ALMOST_WHITE = 0xfefefeff
BLACK = 0x000000ff

describe Compix do
  describe '#find_subimage' do
    let(:image) { OilyPNG::Canvas.new(3, 3, WHITE) }
    let(:threshold) { 0 }
    subject(:subimage_match) { Compix::find_subimage(subimage, image, {threshold: threshold})}

    context "Subimage self of self" do
      let(:subimage) { image }
      it "finds subimage" do
        expected_match = Compix::SubimageMatch.new(0, 0, 9, 9, 3, 3)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Subimage of exact same color" do
      let(:subimage) { OilyPNG::Canvas.new(2, 2, WHITE) }
      it "finds a subimage of exact same color" do
        expected_match = Compix::SubimageMatch.new(0, 0, 4, 4, 2, 2)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Subimage not within threshold" do
      let(:subimage) { OilyPNG::Canvas.new(2, 2, ALMOST_WHITE) }
      it "does not find a subimage" do
        expected_match = Compix::SubimageMatch.new(-1, -1, 4, 0, 2, 2)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Subimage not within a large threshold" do
      let(:subimage) { OilyPNG::Canvas.new(2, 2, BLACK) }
      let(:threshold) { 250 }
      it "does not find a subimage" do
        expected_match = Compix::SubimageMatch.new(-1, -1, 4, 0, 2, 2)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Subimage is within threshold" do
      let(:subimage) { OilyPNG::Canvas.new(2, 2, ALMOST_WHITE) }
      let(:threshold) { 1 }
      it "finds a subimage" do
        expected_match = Compix::SubimageMatch.new(0, 0, 4, 4, 2, 2)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Subimage which is not in image" do
      let(:subimage) { OilyPNG::Canvas.new(2, 2, BLACK) }
      it "does not find a subimage" do
        expected_match = Compix::SubimageMatch.new(-1, -1, 4, 0, 2, 2)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Subimage is larger than source image" do
      let(:subimage) { OilyPNG::Canvas.new(4, 4, WHITE) }
      it "does not find a subimage" do
        expected_match = Compix::SubimageMatch.new(-1, -1, 16, 0, 2, 2)
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
        expected_match = Compix::SubimageMatch.new(1, 1, 4, 4, 2, 2)
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
        expected_match = Compix::SubimageMatch.new(2, 2, 4, 4, 2, 2)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "Without the optional options-hash" do
      let(:subimage_white) { subimage = OilyPNG::Canvas.new(2,3, WHITE) }
      let(:subimage_almost_white) { subimage = OilyPNG::Canvas.new(2,3, ALMOST_WHITE) }
      it "Finds a subimage which exists" do
        subimage_match = Compix::find_subimage(subimage_white, image)
        expected_match = Compix::SubimageMatch.new(0, 0, 6, 6, 2, 2)
        expect(subimage_match).to eql(expected_match)
      end
      it "Does not find a subimage which does not exist" do
        subimage_match = Compix::find_subimage(subimage_almost_white, image)
        expected_match = Compix::SubimageMatch.new(-1, -1, 6, 0, 2, 2)
        expect(subimage_match).to eql(expected_match)
      end
    end

    context "options values" do
      let(:subimage) { subimage = OilyPNG::Canvas.new(2,2, WHITE) }
      let(:expected_match) { Compix::SubimageMatch.new(0, 0, 4, 4, 2, 2) }
      it "minimum values should work" do
        options_hash = { threshold: 0, pixel_threshold: 0.0, xstart: 0, ystart: 0 }
        subimage_match = Compix::find_subimage(subimage, image, options_hash)
        expect(subimage_match).to eql(expected_match)
      end
      it "maximum values should work" do
        options_hash = { threshold: 255, pixel_threshold: 1.0, xstart: 999, ystart: 999 }
        subimage_match = Compix::find_subimage(subimage, image, options_hash)
        expect(subimage_match).to eql(expected_match)
      end
      it "less than minumum values should not work" do
        options_hash = { threshold: -1, pixel_threshold: -0.1, xstart: -1, ystart: -1 }
        expect{ Compix::find_subimage(subimage, image, options_hash) }.to raise_error(ArgumentError)
      end
      it "more than minumum values should not work" do
        options_hash = { threshold: 256, pixel_threshold: 1.1, xstart: 0, ystart: 0 }
        expect{ Compix::find_subimage(subimage, image, options_hash) }.to raise_error(ArgumentError)
      end
    end
  end

  describe "#find_all_instances_of_subimage" do
    let(:subimage_pixels) {[WHITE, BLACK,
                            BLACK, WHITE ]}
    let(:subimage) { OilyPNG::Canvas.new(2,2, subimage_pixels) }
    let(:pixels) { [WHITE, WHITE, WHITE, WHITE, WHITE,
                    WHITE, WHITE, BLACK, WHITE, WHITE,
                    WHITE, BLACK, WHITE, WHITE, BLACK,
                    WHITE, WHITE, WHITE, BLACK, WHITE] }
    let(:image) { OilyPNG::Canvas.new(5, 4, pixels) }
    let(:threshold) { 0 }
    subject(:subimage_matches) do
      Compix::find_all_instances_of_subimage(subimage, image, {threshold: threshold})
    end

    context "With several instances of subimage" do
      it "finds all matches" do
        expect(subimage_matches.count).to eql(2)
      end
      it "finds correct matches" do
        first_match = Compix::SubimageMatch.new(1, 1, 4, 4, 2, 2)
        second_match = Compix::SubimageMatch.new(3, 2, 4, 4, 2, 2)
        expect(subimage_matches).to eql([first_match, second_match])
      end
    end

    context "With no instances of subimage" do
      let(:subimage) { OilyPNG::Canvas.new(2,2, ALMOST_WHITE) }
      it "Finds no matches" do
        expect(subimage_matches.count).to eql(0)
      end
    end

    context "With possibly overlapping subimages" do
      let(:subimage) { OilyPNG::Canvas.new(2,2, BLACK) }
      let(:pixels) { [WHITE, BLACK, BLACK, WHITE, WHITE,
                      WHITE, BLACK, BLACK, BLACK, BLACK,
                      WHITE, BLACK, BLACK, BLACK, BLACK,
                      WHITE, BLACK, BLACK, WHITE, WHITE] }
      it "Finds correct amount of matches" do
        expect(subimage_matches.count).to eql(3)
      end
      it "Finds correct matches" do
        expected_matches = [ Compix::SubimageMatch.new(1, 0, 4, 4, 2, 2),
                             Compix::SubimageMatch.new(3, 1, 4, 4, 2, 2),
                             Compix::SubimageMatch.new(1, 2, 4, 4, 2, 2) ]
        expect(subimage_matches).to eql(expected_matches)
      end
    end
  end
end
