#pragma once
#ifdef ENABLE_MODERN_MANAGER

#include <QWidget>
#include <QPixmap>

class QLabel;
class QPropertyAnimation;

namespace rpcs3::ui
{
	class BigTile : public QWidget
	{
		Q_OBJECT
	public:
		explicit BigTile(QWidget* parent = nullptr);

		void setPixmap(const QPixmap& pix);
		void setTitle(const QString& title);
		void setSelected(bool selected);

	Q_SIGNALS:
		void activated();

	protected:
		void paintEvent(QPaintEvent* event) override;
		void keyPressEvent(QKeyEvent* event) override;
		void focusInEvent(QFocusEvent* event) override;
		void focusOutEvent(QFocusEvent* event) override;
		void mouseDoubleClickEvent(QMouseEvent* event) override;

	private:
		QLabel* m_image = nullptr;
		QLabel* m_title = nullptr;
		bool m_selected = false;
	};
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
