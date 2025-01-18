#include "QWidgetAdminDnD.h"
#include <QCoreApplication>
#include <QMimeData>
#include <QDebug>
#include <windows.h>


// constructor
QWidgetAdminDnD::QWidgetAdminDnD(QWidget* _Parent) noexcept : QWidget(_Parent)
{
	setWindowTitle(tr(u8"XCC-管理员拖放"));

	QCoreApplication::instance()->installNativeEventFilter(this);
	QObject::connect(this, &QWidgetAdminDnD::signalAdminDrop, this, &QWidgetAdminDnD::receiveAdminDrop, Qt::QueuedConnection);

	setAcceptDrops(true);
	auto		vHwnd = (HWND)winId();
	DragAcceptFiles(vHwnd, true);
	RevokeDragDrop(vHwnd);
}

// destructor
QWidgetAdminDnD::~QWidgetAdminDnD() noexcept = default;



// [event] 消息过滤
bool QWidgetAdminDnD::nativeEventFilter(const QByteArray& _EventType, void* _Message, long* _Result)
{
	Q_UNUSED(_EventType);
	Q_UNUSED(_Result);
	auto		vMsg = (MSG*)_Message;
	if(vMsg && vMsg->message == WM_DROPFILES)
	{
		auto		vFileArray = QList<QUrl>();
		wchar_t 	vFilePath[1024] = {0};
		auto		vCount = DragQueryFileW((HDROP)vMsg->wParam, 0xFFFFFFFF, nullptr, 0);
		for(auto vIndex = 0u; vIndex < vCount; ++vIndex)
		{
			auto		vPathSize = DragQueryFileW((HDROP) vMsg->wParam, vIndex, vFilePath, 1024 - 1);
			vFilePath[vPathSize] = {0};
			vFileArray.push_back(QUrl::fromLocalFile(QString::fromStdWString(vFilePath)));
		}
		POINT 		vPoint;
		DragQueryPoint((HDROP)vMsg->wParam, &vPoint);
		DragFinish((HDROP)vMsg->wParam);

		emit signalAdminDrop((WId)vMsg->hwnd, QPoint((int)vPoint.x, (int)vMsg->pt.y), vFileArray);
		return true;
	}
	return false;
}

// [event] 拖入
void QWidgetAdminDnD::dragEnterEvent(QDragEnterEvent* _Event)
{
	auto		vMimeData = _Event->mimeData();
	if(vMimeData)
	{
		debug("dragEnterEvent: <<<<<<<<<<");
		auto		vUrls = vMimeData->urls();
		for(const auto & vIterator : vUrls)
		{
			if(vIterator.isLocalFile())
			{
				debug(QString("dragEnterEvent: %1").arg(vIterator.toLocalFile()));
				_Event->accept();
			}
		}
		debug("dragEnterEvent: >>>>>>>>>>");
	}
	QWidget::dragEnterEvent(_Event);
}

// [event] 移动
void QWidgetAdminDnD::dragMoveEvent(QDragMoveEvent* _Event)
{
	QWidget::dragMoveEvent(_Event);
}

// [event] 放下
void QWidgetAdminDnD::dropEvent(QDropEvent* _Event)
{
	auto		vMimeData = _Event->mimeData();
	if(vMimeData)
	{
		debug("dragEnterEvent: <<<<<<<<<<");
		auto		vUrls = vMimeData->urls();
		for(const auto & vIterator : vUrls)
		{
			if(vIterator.isLocalFile())
			{
				debug(QString("dropEvent: %1").arg(vIterator.toLocalFile()));
				_Event->accept();
			}
		}
		debug("dragEnterEvent: >>>>>>>>>>");
	}
	QWidget::dropEvent(_Event);
}



// [opt] debug
void QWidgetAdminDnD::debug(const QString& _Text) noexcept
{
	auto		vText = _Text.toStdString();
	std::printf("%s\n", vText.data());
}



// [receive] 拖放事件
void QWidgetAdminDnD::receiveAdminDrop(WId _Hwnd, const QPoint& _Point, const QList<QUrl>& _Urls) noexcept
{
	// 查找指定窗口
	auto		vWidget = QWidget::find(_Hwnd);
	if(nullptr == vWidget || !vWidget->acceptDrops())
	{
		return;
	}

	// 发送事件
	QMimeData		vMimeData;
	vMimeData.setUrls(_Urls);
	QDragEnterEvent		vDragEnterEvent(_Point, Qt::CopyAction, &vMimeData, Qt::LeftButton, Qt::NoModifier);
	QCoreApplication::sendEvent(vWidget, &vDragEnterEvent);

	QDragMoveEvent		vDragMoveEvent(_Point, Qt::CopyAction, &vMimeData, Qt::LeftButton, Qt::NoModifier);
	QCoreApplication::sendEvent(vWidget, &vDragMoveEvent);

	QDropEvent		vDropEvent(_Point, Qt::CopyAction, &vMimeData, Qt::LeftButton, Qt::NoModifier, QEvent::Drop);
	QCoreApplication::sendEvent(vWidget, &vDropEvent);
}
