#pragma once
#ifdef ENABLE_MODERN_MANAGER

#include <QObject>
#include <QWidget>

namespace rpcs3::ui
{
	class FocusNav : public QObject
	{
		Q_OBJECT
	public:
		static void install(QWidget* root);

	Q_SIGNALS:
		void activated();

	protected:
		bool eventFilter(QObject* obj, QEvent* ev) override;

	private:
		explicit FocusNav(QWidget* root);
		QWidget* m_root = nullptr;
		QWidget* nextWidget(QWidget* current, Qt::Key key) const;
		static void setPolicies(QWidget* w);
	};
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
