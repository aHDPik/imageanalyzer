#include "ImageLibrary.h"

#include <imagelib.h>

using namespace System::Windows::Media::Imaging;
using namespace System::Windows::Media;
using namespace System::Collections::Generic;

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

	List<Detection>^ ImageLibrary::Detect(WriteableBitmap^ img)
	{
		FormatConvertedBitmap^ bgr24 = ToBgr24(img);
		array<unsigned char>^ pixels = gcnew array<unsigned char>(img->PixelHeight * img->PixelWidth * 3);
		bgr24->CopyPixels(pixels, img->PixelWidth * 3, 0);

		pin_ptr<unsigned char> pixelsPin = &pixels[0];
		unsigned char* pixelsNative = pixelsPin;

		std::vector<imagelib::Detection> resArr = imagelib::detect(pixelsNative, img->PixelWidth, img->PixelHeight);
		List<Detection>^ outArr = gcnew List<Detection>();
		for (int i = 0; i < resArr.size(); i++) {
			Detection out;
			out.x = resArr[i].x;
			out.y = resArr[i].y;
			out.width = resArr[i].width;
			out.height = resArr[i].height;
			outArr->Add(out);
		}

		return outArr;
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

	System::Windows::Media::Imaging::WriteableBitmap^ ImageLibrary::ApplyMatrix(System::Windows::Media::Imaging::WriteableBitmap^ img, Matrix m)
	{
		FormatConvertedBitmap^ bgr24 = ToBgr24(img);
		array<unsigned char>^ pixels = gcnew array<unsigned char>(img->PixelHeight * img->PixelWidth * 3);
		bgr24->CopyPixels(pixels, img->PixelWidth * 3, 0);

		pin_ptr<unsigned char> pixelsPin = &pixels[0];
		unsigned char* pixelsNative = pixelsPin;

		imagelib::Matrix mNative;
		for (int i = 0; i < m.arr1->Length; i++) {
			double val = m.arr1[i];
			mNative.arr1.push_back(val);
		}
		for (int i = 0; i < m.arr2->Length; i++) {
			double val = m.arr2[i];
			mNative.arr2.push_back(val);
		}

		mNative.width = m.arr1->Length;
		mNative.height = m.arr2->Length;
		mNative.name = "Gui input";
		imagelib::modify_image(pixelsNative, img->PixelWidth, img->PixelHeight, mNative);
		WriteableBitmap^ res = gcnew WriteableBitmap(img->PixelWidth, img->PixelHeight, 92, 92, PixelFormats::Bgr24, nullptr);
		res->WritePixels(System::Windows::Int32Rect(0, 0, img->PixelWidth, img->PixelHeight), pixels, img->PixelWidth * 3, 0);
		return res;
	}
}