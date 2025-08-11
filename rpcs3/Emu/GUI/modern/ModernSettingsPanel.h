#pragma once
#ifdef ENABLE_MODERN_MANAGER

#include <QDialog>
#include <memory>

class QCheckBox;
class QPushButton;
class gui_settings;

namespace rpcs3::ui
{
	class ModernSettingsPanel : public QDialog
	{
		Q_OBJECT
	public:
		explicit ModernSettingsPanel(std::shared_ptr<gui_settings> settings, QWidget* parent = nullptr);

	private Q_SLOTS:
		void save();

	private:
		std::shared_ptr<gui_settings> m_settings;
		QCheckBox* m_start_console = nullptr;
		QCheckBox* m_large_text = nullptr;
	};
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
