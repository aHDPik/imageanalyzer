#include "ImageLibrary.h"

#include <imagelib.h>

using namespace System::Windows::Media::Imaging;
using namespace System::Windows::Media;

namespace ImageLib {




	Detection ImageLibrary::Detect(BitmapImage^ img)
	{
		FormatConvertedBitmap^ newFormatedBitmapSource = gcnew FormatConvertedBitmap();

		// BitmapSource objects like FormatConvertedBitmap can only have their properties
		// changed within a BeginInit/EndInit block.
		newFormatedBitmapSource->BeginInit();

		// Use the BitmapSource object defined above as the source for this new
		// BitmapSource (chain the BitmapSource objects together).
		newFormatedBitmapSource->Source = img;

		// Set the new format to Gray32Float (grayscale).
		newFormatedBitmapSource->DestinationFormat = PixelFormats::Bgr24;
		newFormatedBitmapSource->EndInit();
		array<unsigned char>^ pixels = gcnew array<unsigned char>(img->PixelHeight * img->PixelWidth * 3);
		newFormatedBitmapSource->CopyPixels(pixels, img->PixelWidth * 3, 0);

		pin_ptr<unsigned char> pixelsPin = &pixels[0];
		unsigned char* pixelsNative = pixelsPin;

		
		//тут должна быть функция шума 
		//noise(фото, img->Width, img->Height, 0.1/*10%*/ )

		imagelib::Detection res = imagelib::detect(pixelsNative, img->Width, img->Height);
		Detection out;
		out.x = res.x;
		out.y = res.y;
		out.width = res.width;
		out.height = res.height;
		return out;
	}

}