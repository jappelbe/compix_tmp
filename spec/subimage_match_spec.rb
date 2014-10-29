require 'spec_helper'

describe Compix::SubimageMatch do

  describe "#initialize" do
    let(:x_coord) { 5 }
    let(:y_coord) { 17 }
    let(:width) { 80 }
    let(:height) { 100 }
    let(:limit_for_match) { 0.85 }
    subject do
      Compix::SubimageMatch.new(x_coord, y_coord, compared_pixels,
                                matched_pixels, width, height, limit_for_match)
    end

    context "Proper arguments" do
      let(:compared_pixels) { 7 }
      let(:matched_pixels) { 6 }
      it "initializes object correctly" do
        expect(subject.coordinate_x).to eql(x_coord)
        expect(subject.coordinate_y).to eql(y_coord)
        expect(subject.pixels_compared).to eql(compared_pixels)
        expect(subject.pixels_matched).to eql(matched_pixels)
      end
      it "is a match" do
        expect(subject.match?).to be true
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
    subject { Compix::SubimageMatch.new(0, 0, compared_pixels, matched_pixels, 3, 3 ) }

    context "perfect match" do
      let(:compared_pixels) { 9 }
      let(:matched_pixels) { 9 }
      it "gives correct percentage" do
        expect(subject.match_percentage).to eql( matched_pixels.to_f / compared_pixels )
      end
      it "is a match" do
        expect(subject.match?).to be true
      end
    end

    context "not perfect match" do
      let(:compared_pixels) { 9 }
      let(:matched_pixels) { 5 }
      it "gives proper percentage" do
        expect( subject.match_percentage ).to eql( matched_pixels.to_f / compared_pixels )
      end
      it "is not a match" do
        expect(subject.match?).to be false
      end
    end

    context "no matches" do
      let(:compared_pixels) { 9 }
      let(:matched_pixels) { 0 }
      it "gives proper percentage" do
        expect( subject.match_percentage ).to eql( matched_pixels.to_f / compared_pixels )
      end
      it "is not a match" do
        expect(subject.match?).to be false
      end
    end

    context "nothing to compare" do
      let(:compared_pixels) { 0 }
      let(:matched_pixels) { 0 }
      it "gives proper percentage" do
        expect( subject.match_percentage ).to eql( 0 )
      end
      it "is not a match" do
        expect(subject.match?).to be false
      end
    end
  end
end
