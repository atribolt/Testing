// Ffmpeg.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <chrono>

#include "lz4.h"
#include "lz4xx.h"

size_t ScreenX = GetDeviceCaps(GetDC(0), HORZRES);
size_t ScreenY = GetDeviceCaps(GetDC(0), VERTRES);
size_t sz   = ScreenX * ScreenY * 3;

auto ScreenCap(BYTE* buffer, int width, int height) {
   //BYTE* ScreenData = new BYTE[3 * width * height];
   
   HDC hdc              = GetDC(GetDesktopWindow());
   HDC hdcMem           = CreateCompatibleDC(hdc);
   HBITMAP hBitmap      = CreateCompatibleBitmap(hdc, width, height);
   BITMAPINFOHEADER bmi = { 0 };
   bmi.biSize           = sizeof(BITMAPINFOHEADER);
   bmi.biPlanes         = 1;
   bmi.biBitCount       = 24;
   bmi.biWidth          = width;
   bmi.biHeight         = -height;
   bmi.biCompression    = BI_RGB;
   bmi.biSizeImage      = width * height;
   SelectObject(hdcMem, hBitmap);
   BitBlt(hdcMem, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
   GetDIBits(hdc, hBitmap, 0, height, buffer, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

   return std::pair{ buffer, width * height * 3 };
}

auto GetTime() {
   auto tm = std::chrono::high_resolution_clock::now();
   return std::chrono::duration_cast<std::chrono::milliseconds>(tm.time_since_epoch());
}

int main()
{
   std::ofstream f("screen_compress", std::ios::binary);
   BYTE* screen = new BYTE[sz];

   char* comress_data = new char[sz];

   auto start = GetTime();

   int i = 100;
   while (i--) {
      ScreenCap(screen, ScreenX, ScreenY);
      size_t i = sz;

      auto size_r = LZ4_compress((char*)screen, comress_data, sz);

      while (size_r--) {
         f << comress_data[size_r + 1];
      }
   }

   std::cout << "Time: " << (GetTime() - start).count() << '\n';

   f.close();
   system("pause");
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
