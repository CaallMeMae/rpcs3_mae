#ifdef ENABLE_MODERN_MANAGER

#include "ModernSettingsPanel.h"
#include "rpcs3qt/gui_settings.h"

#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>

namespace rpcs3::ui
{
	ModernSettingsPanel::ModernSettingsPanel(std::shared_ptr<gui_settings> settings, QWidget* parent)
		: QDialog(parent), m_settings(std::move(settings))
	{
		setWindowTitle(tr("Settings"));
		auto* layout = new QVBoxLayout(this);

		m_start_console = new QCheckBox(tr("Start in Console Mode"), this);
		m_start_console->setChecked(m_settings->GetValue("modern", "start_console_mode", false).toBool());
		layout->addWidget(m_start_console);

		m_large_text = new QCheckBox(tr("Large Text mode"), this);
		m_large_text->setChecked(m_settings->GetValue("modern", "large_text", false).toBool());
		layout->addWidget(m_large_text);

		QPushButton* online = new QPushButton(tr("Online Settings..."), this);
		layout->addWidget(online);
		connect(online, &QPushButton::clicked, this, []() { /* placeholder */ });

		auto* ok = new QPushButton(tr("OK"), this);
		layout->addWidget(ok);
		connect(ok, &QPushButton::clicked, this, &ModernSettingsPanel::save);
	}

	void ModernSettingsPanel::save()
	{
		m_settings->SetValue("modern", "start_console_mode", m_start_console->isChecked());
		m_settings->SetValue("modern", "large_text", m_large_text->isChecked());
		accept();
	}
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
