#ifdef ENABLE_MODERN_MANAGER

#include "BigTile.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QPainter>

namespace rpcs3::ui
{
	BigTile::BigTile(QWidget* parent)
		: QWidget(parent)
	{
		setFocusPolicy(Qt::StrongFocus);
		auto* layout = new QVBoxLayout(this);
		layout->setContentsMargins(4, 4, 4, 4);
		layout->setSpacing(4);
		layout->addStretch();
		m_image = new QLabel(this);
		m_image->setAlignment(Qt::AlignCenter);
		layout->addWidget(m_image);
		m_title = new QLabel(this);
		m_title->setAlignment(Qt::AlignCenter);
		layout->addWidget(m_title);
	}

	void BigTile::setPixmap(const QPixmap& pix)
	{
		m_image->setPixmap(pix);
	}

	void BigTile::setTitle(const QString& title)
	{
		m_title->setText(title);
	}

	void BigTile::setSelected(bool selected)
	{
		if (m_selected == selected)
			return;
		m_selected = selected;
		update();
	}

	void BigTile::paintEvent(QPaintEvent* ev)
	{
		QWidget::paintEvent(ev);
		if (m_selected)
		{
			QPainter p(this);
			p.setPen(QPen(Qt::yellow, 2));
			p.drawRect(rect().adjusted(1, 1, -1, -1));
		}
	}

	void BigTile::keyPressEvent(QKeyEvent* event)
	{
		if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Space)
		{
			Q_EMIT activated();
			event->accept();
			return;
		}
		QWidget::keyPressEvent(event);
	}

	void BigTile::focusInEvent(QFocusEvent* event)
	{
		setSelected(true);
		QWidget::focusInEvent(event);
	}

	void BigTile::focusOutEvent(QFocusEvent* event)
	{
		setSelected(false);
		QWidget::focusOutEvent(event);
	}

	void BigTile::mouseDoubleClickEvent(QMouseEvent* event)
	{
		if (event->button() == Qt::LeftButton)
		{
			Q_EMIT activated();
		}
		QWidget::mouseDoubleClickEvent(event);
	}
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
