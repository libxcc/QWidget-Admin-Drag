#include "QWidgetAdminDnD.h"
#include <QApplication>
#include <windows.h>


// 设置DPI
static void x_main_set_dpi() noexcept
{
	SetProcessDPIAware();

	auto		vDesktopDC = ::GetDC(nullptr);
	auto		vDPI_W = ::GetDeviceCaps(vDesktopDC, LOGPIXELSX);
	::ReleaseDC(nullptr, vDesktopDC);

	auto		vScanRatio = (double)vDPI_W / 96.0f;
	if(vScanRatio <= 0.4f)
	{
		qputenv("QT_SCALE_FACTOR", "0.25");
	}
	else if(vScanRatio <= 0.6f)
	{
		qputenv("QT_SCALE_FACTOR", "0.5");
	}
	else if(vScanRatio <= 0.9f)
	{
		qputenv("QT_SCALE_FACTOR", "0.75");
	}
	else if(vScanRatio <= 1.1f)
	{
		qputenv("QT_SCALE_FACTOR", "1.0");
	}
	else if(vScanRatio <= 1.4f)
	{
		qputenv("QT_SCALE_FACTOR", "1.25");
	}
	else if(vScanRatio <= 1.6f)
	{
		qputenv("QT_SCALE_FACTOR", "1.5");
	}
	else if(vScanRatio <= 1.9f)
	{
		qputenv("QT_SCALE_FACTOR", "1.75");
	}
	else if(vScanRatio <= 2.1f)
	{
		qputenv("QT_SCALE_FACTOR", "2.0");
	}
	else if(vScanRatio <= 2.4f)
	{
		qputenv("QT_SCALE_FACTOR", "2.25");
	}
	else if(vScanRatio <= 2.6f)
	{
		qputenv("QT_SCALE_FACTOR", "2.5");
	}
	else
	{
		qputenv("QT_SCALE_FACTOR", "3.0");
	}
}

// 入口函数
int main(int _Argc, char** _Argv)
{
	OleInitialize(nullptr);

	AllocConsole();
	SetConsoleTitleW(L"DK-Core调试模式");
	SetWindowPos(GetConsoleWindow(), HWND_TOP, 10, 10, 0, 0, SWP_NOSIZE);
	auto		vHandle = std::freopen("CONOUT$", "w+t", stdout);

	x_main_set_dpi();
	// OpenGL渲染级别		(不启用时可能：在某些旧显卡机器上启动程序时，浏览器页面显示花屏)
	QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, true);
	// 高质量图片
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
	// 创建本机窗口
	QCoreApplication::setAttribute(Qt::AA_NativeWindows, true);

	ChangeWindowMessageFilter(WM_DROPFILES, 1);
	ChangeWindowMessageFilter(WM_COPYDATA, 1);
	ChangeWindowMessageFilter(0x0049, 1);

	QApplication		vApplication(_Argc, _Argv);

	auto		vWindow = new(std::nothrow) QWidgetAdminDnD(nullptr);
	vWindow->show();
	QApplication::exec();

	if(vHandle)
	{
		std::fclose(vHandle);
		vHandle = nullptr;
	}
	FreeConsole();
	SendMessageW(GetConsoleWindow(), WM_CLOSE, NULL, NULL);

	OleUninitialize();
	return 0;
}
