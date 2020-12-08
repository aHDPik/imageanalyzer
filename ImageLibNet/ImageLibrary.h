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
	public:
		static Detection Detect(System::Windows::Media::Imaging::BitmapImage^ img);



	};

}

