#pragma once
#include<vector>

namespace ImageLib {

	public value struct Detection {
	public:
		int x;
		int y;
		int width;
		int height;
	};

	public value struct Matrix {
	public:
		array<double>^ arr1;
		array<double>^ arr2;
	};


	public ref class ImageLibrary
	{
	private:
		static System::Windows::Media::Imaging::FormatConvertedBitmap^ ToBgr24(System::Windows::Media::Imaging::WriteableBitmap^ img);
	public:
		static System::Collections::Generic::List<Detection>^ Detect(System::Windows::Media::Imaging::WriteableBitmap^ img, int minCountPicsel, int minSquare);
		static System::Windows::Media::Imaging::WriteableBitmap^ Noise(System::Windows::Media::Imaging::WriteableBitmap^ img, double percent);
		static System::Windows::Media::Imaging::WriteableBitmap^ ApplyMatrix(System::Windows::Media::Imaging::WriteableBitmap^ img, Matrix m);
	};

}

