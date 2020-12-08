using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using ImageLib;

namespace ImageAnalyzer
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private BitmapImage img;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void openFileMenu_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "Файлы рисунков (*.bmp, *.jpg, *.png)|*.bmp;*.jpg;*.png";
            if (ofd.ShowDialog().Value)
            {
                img = new BitmapImage(new Uri(ofd.FileName));
                image.Source = img;
                Detection det = ImageLibrary.Detect(img);
                MessageBox.Show($"x={det.x}, y={det.y}, width={det.width}, height={det.height}");
                FormatConvertedBitmap  bmp32 = new FormatConvertedBitmap();

                // BitmapSource objects like FormatConvertedBitmap can only have their properties
                // changed within a BeginInit/EndInit block.
                bmp32.BeginInit();

                // Use the BitmapSource object defined above as the source for this new
                // BitmapSource (chain the BitmapSource objects together).
                bmp32.Source = img;

                // Set the new format to Gray32Float (grayscale).
                bmp32.DestinationFormat = PixelFormats.Bgr32;
                bmp32.EndInit();
                WriteableBitmap wbmp = new WriteableBitmap(bmp32);
                wbmp.DrawRectangle(det.x, det.y, det.x + det.width, det.y + det.height, Colors.Red);
                image.Source = wbmp;
            }
        }
    }
}
