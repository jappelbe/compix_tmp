require 'spec_helper'

describe Compix::SubimageMatch do

  describe "#initialize" do
    let(:x_coord) { 5 }
    let(:y_coord) { 17 }
    subject { Compix::SubimageMatch.new(x_coord, y_coord, compared_pixels, matched_pixels) }

    context "Proper arguments" do
      let(:compared_pixels) { 7 }
      let(:matched_pixels) { 5 }
      it "initializes object correctly" do
        expect(subject.coordinate_x).to eql(x_coord)
        expect(subject.coordinate_y).to eql(y_coord)
        expect(subject.pixels_compared).to eql(compared_pixels)
        expect(subject.pixels_matched).to eql(matched_pixels)
      end
    end

    context "Bad arguments" do
      let(:compared_pixels) { 5 }
      let(:matched_pixels) { 6 }
      it "Raises argument error" do
        expect{ subject }.to raise_error(ArgumentError)
      end
    end
  end

  describe "#match_percentage" do
    subject { Compix::SubimageMatch.new(0, 0, compared_pixels, matched_pixels ) }

    context "perfect match" do
      let(:compared_pixels) { 9 }
      let(:matched_pixels) { 9 }
      it "gives correct percentage" do
        expect(subject.match_percentage).to eql( matched_pixels.to_f / compared_pixels )
      end
    end

    context "not perfect match" do
      let(:compared_pixels) { 9 }
      let(:matched_pixels) { 5 }
      it "gives proper percentage" do
        expect( subject.match_percentage ).to eql( matched_pixels.to_f / compared_pixels )
      end
    end

    context "no matches" do
      let(:compared_pixels) { 9 }
      let(:matched_pixels) { 0 }
      it "gives proper percentage" do
        expect( subject.match_percentage ).to eql( matched_pixels.to_f / compared_pixels )
      end
    end

    context "nothing to compare" do
      let(:compared_pixels) { 0 }
      let(:matched_pixels) { 0 }
      it "gives proper percentage" do
        expect( subject.match_percentage ).to eql( 0 )
      end
    end
  end
end
