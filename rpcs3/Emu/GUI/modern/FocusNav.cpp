#ifdef ENABLE_MODERN_MANAGER

#include "FocusNav.h"

#include <QKeyEvent>
#include <QWidget>
#include <QApplication>

namespace rpcs3::ui
{
	FocusNav::FocusNav(QWidget* root)
		: QObject(root), m_root(root)
	{
		setPolicies(root);
		root->installEventFilter(this);
	}

	void FocusNav::install(QWidget* root)
	{
		new FocusNav(root);
	}

	void FocusNav::setPolicies(QWidget* w)
	{
		w->setFocusPolicy(Qt::StrongFocus);
		for (auto* child : w->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly))
		{
			setPolicies(child);
		}
	}

	QWidget* FocusNav::nextWidget(QWidget* current, Qt::Key key) const
	{
		if (!m_root)
			return nullptr;
		const QRect cur = current->geometry().translated(current->mapTo(m_root, QPoint()));
		QWidget* best = nullptr;
		int best_dist = std::numeric_limits<int>::max();

		const auto widgets = m_root->findChildren<QWidget*>();
		for (QWidget* w : widgets)
		{
			if (!w->isVisible() || w->focusPolicy() == Qt::NoFocus || w == current)
				continue;
			QRect r = w->geometry().translated(w->mapTo(m_root, QPoint()));
			QPoint delta = r.center() - cur.center();
			switch (key)
			{
			case Qt::Key_Left:
				if (delta.x() >= 0)
					continue;
				break;
			case Qt::Key_Right:
				if (delta.x() <= 0)
					continue;
				break;
			case Qt::Key_Up:
				if (delta.y() >= 0)
					continue;
				break;
			case Qt::Key_Down:
				if (delta.y() <= 0)
					continue;
				break;
			default: break;
			}
			int dist = delta.x() * delta.x() + delta.y() * delta.y();
			if (dist < best_dist)
			{
				best_dist = dist;
				best = w;
			}
		}
		return best;
	}

	bool FocusNav::eventFilter(QObject* obj, QEvent* ev)
	{
		if (ev->type() == QEvent::KeyPress)
		{
			QKeyEvent* ke = static_cast<QKeyEvent*>(ev);
			QWidget* fw = m_root->focusWidget();
			if (!fw)
				fw = m_root;
			switch (ke->key())
			{
			case Qt::Key_Left:
			case Qt::Key_Right:
			case Qt::Key_Up:
			case Qt::Key_Down:
			{
				if (QWidget* next = nextWidget(fw, static_cast<Qt::Key>(ke->key())))
				{
					next->setFocus(Qt::OtherFocusReason);
					return true;
				}
				break;
			}
			case Qt::Key_Return:
			case Qt::Key_Enter:
			case Qt::Key_Space:
				Q_EMIT activated();
				break;
			default:
				break;
			}
		}
		return QObject::eventFilter(obj, ev);
	}
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
