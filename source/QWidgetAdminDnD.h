#ifndef			_Q_WIDGET_ADMIN_DRAG_H_
#define			_Q_WIDGET_ADMIN_DRAG_H_

#include <QWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QAbstractNativeEventFilter>



// 拖放演示窗口
class QWidgetAdminDnD : public QWidget, public QAbstractNativeEventFilter
{
Q_OBJECT

public:
	// constructor
	explicit QWidgetAdminDnD(QWidget* _Parent = nullptr) noexcept;

	// destructor
	~QWidgetAdminDnD() noexcept override;

protected:
	// [event] 消息过滤
	bool nativeEventFilter(const QByteArray& _EventType, void* _Message, long* _Result) override;

	// [event] 拖入
	void dragEnterEvent(QDragEnterEvent* _Event) override;

	// [event] 移动
	void dragMoveEvent(QDragMoveEvent* _Event) override;

	// [event] 放下
	void dropEvent(QDropEvent* _Event) override;

public:
	// [opt] debug
	static void debug(const QString& _Text) noexcept;

signals:
	// [signal] 拖放事件
	void signalAdminDrop(WId _Hwnd, const QPoint& _Point, const QList<QUrl>& _Urls);

public slots:
	// [receive] 拖放事件
	virtual void receiveAdminDrop(WId _Hwnd, const QPoint& _Point, const QList<QUrl>& _Urls) noexcept final;
};


#endif
