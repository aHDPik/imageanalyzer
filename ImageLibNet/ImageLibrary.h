#pragma once

namespace ImageLib {

	public value struct Detection {
	public:
		int x;
		int y;
		int width;
		int height;
	};

	public ref class ImageLibrary
	{
	private:
		static System::Windows::Media::Imaging::FormatConvertedBitmap^ ToBgr24(System::Windows::Media::Imaging::WriteableBitmap^ img);
	public:
		static Detection Detect(System::Windows::Media::Imaging::WriteableBitmap^ img);
		static System::Windows::Media::Imaging::WriteableBitmap^ Noise(System::Windows::Media::Imaging::WriteableBitmap^ img, double percent);
		static System::Windows::Media::Imaging::WriteableBitmap^ Matrix(System::Windows::Media::Imaging::WriteableBitmap^ img, double percent);
	};

}

