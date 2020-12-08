#include "ImageLibrary.h"

#include <imagelib.h>

using namespace System::Windows::Media::Imaging;
using namespace System::Windows::Media;

namespace ImageLib {


	FormatConvertedBitmap^ ImageLibrary::ToBgr24(WriteableBitmap^ img) {
		FormatConvertedBitmap^ bgr24 = gcnew FormatConvertedBitmap();

		// BitmapSource objects like FormatConvertedBitmap can only have their properties
		// changed within a BeginInit/EndInit block.
		bgr24->BeginInit();

		// Use the BitmapSource object defined above as the source for this new
		// BitmapSource (chain the BitmapSource objects together).
		bgr24->Source = img;

		// Set the new format to Gray32Float (grayscale).
		bgr24->DestinationFormat = PixelFormats::Bgr24;
		bgr24->EndInit();
		return bgr24;
	}

	Detection ImageLibrary::Detect(WriteableBitmap^ img)
	{
		FormatConvertedBitmap^ bgr24 = ToBgr24(img);
		array<unsigned char>^ pixels = gcnew array<unsigned char>(img->PixelHeight * img->PixelWidth * 3);
		bgr24->CopyPixels(pixels, img->PixelWidth * 3, 0);

		pin_ptr<unsigned char> pixelsPin = &pixels[0];
		unsigned char* pixelsNative = pixelsPin;

		
		//��� ������ ���� ������� ���� 
		//noise(����, img->Width, img->Height, 0.1/*10%*/ )

		imagelib::Detection res = imagelib::detect(pixelsNative, img->PixelWidth, img->PixelHeight);
		Detection out;
		out.x = res.x;
		out.y = res.y;
		out.width = res.width;
		out.height = res.height;
		return out;
	}

	WriteableBitmap^ ImageLibrary::Noise(WriteableBitmap^ img, double percent)
	{
		FormatConvertedBitmap^ bgr24 = ToBgr24(img);
		array<unsigned char>^ pixels = gcnew array<unsigned char>(img->PixelHeight * img->PixelWidth * 3);
		bgr24->CopyPixels(pixels, img->PixelWidth * 3, 0);

		pin_ptr<unsigned char> pixelsPin = &pixels[0];
		unsigned char* pixelsNative = pixelsPin;

		imagelib::noise(pixelsNative, img->PixelWidth, img->PixelHeight, percent);

		WriteableBitmap^ res = gcnew WriteableBitmap(img->PixelWidth, img->PixelHeight, 92, 92, PixelFormats::Bgr24, nullptr);
		res->WritePixels(System::Windows::Int32Rect(0, 0, img->PixelWidth, img->PixelHeight), pixels, img->PixelWidth * 3, 0);
		return res;
	}

}