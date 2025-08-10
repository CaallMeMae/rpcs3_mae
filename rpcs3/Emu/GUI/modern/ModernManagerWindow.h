#pragma once

#include <QMainWindow>

namespace rpcs3::ui
{
	class ModernManagerWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		explicit ModernManagerWindow(QWidget* parent = nullptr);
		~ModernManagerWindow() override;
	};
} // namespace rpcs3::ui
