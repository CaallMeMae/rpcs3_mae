#ifdef ENABLE_MODERN_MANAGER

#include "ModernManagerWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QItemSelectionModel>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <memory>

#include "Emu/System.h"
#include "Utilities/File.h"
#include "util/logs.hpp"
#include "rpcs3qt/pad_settings_dialog.h"
#include "rpcs3qt/gui_settings.h"
#include "ModernTelemetry.h"

LOG_CHANNEL(modern_manager_log, "ModernManager");

namespace rpcs3::ui
{
	ModernManagerWindow::ModernManagerWindow(QWidget* parent)
		: QMainWindow(parent)
	{
		searchBar = new QLineEdit(this);
		btnSettings = new QPushButton(tr("Settings"), this);
		lblFirmwareStatus = new QLabel(tr("FW Unknown"), this);

		btnAddGame = new QPushButton(tr("Add Game"), this);
		btnAddGame->setToolTip(tr("Add a new game to the library"));
		btnOptimize = new QPushButton(tr("Optimize"), this);
		btnOptimize->setToolTip(tr("Optimize selected game"));
		btnPrebuildShaders = new QPushButton(tr("Prebuild Shaders"), this);
		btnPrebuildShaders->setToolTip(tr("Precompile shaders for selected game"));
		btnControllerSetup = new QPushButton(tr("Controller Setup"), this);
		btnControllerSetup->setToolTip(tr("Configure controllers"));
		btnUpdateAll = new QPushButton(tr("Update All"), this);
		btnUpdateAll->setToolTip(tr("Refresh the library"));

		gameGrid = new QListView(this);
		gameGrid->setViewMode(QListView::IconMode);
		gameGrid->setWrapping(true);
		gameGrid->setResizeMode(QListView::Adjust);
		gameGrid->setIconSize({96, 96});

		lblGameTitle = new QLabel(this);
		lblLastPlayed = new QLabel(this);
		lblHoursPlayed = new QLabel(this);
		lblStatus = new QLabel(this);
		btnPlay = new QPushButton(tr("Play"), this);
		btnPlay->setToolTip(tr("Boot selected game"));

		m_settings = std::make_shared<gui_settings>();
		m_show_favorites_only = m_settings->GetValue("ui/modern_manager", "show_favorites_only", false).toBool();
		const QByteArray geom = m_settings->GetValue("ui/modern_manager", "window_geometry", QByteArray()).toByteArray();
		if (!geom.isEmpty())
		{
			restoreGeometry(geom);
		}

		m_model = new ModernLibraryModel(this);
		gameGrid->setModel(m_model);

		auto* central = new QWidget(this);
		auto* mainLayout = new QVBoxLayout(central);
		mainLayout->setContentsMargins(4, 4, 4, 4);

		auto* topLayout = new QHBoxLayout();
		topLayout->setContentsMargins(0, 0, 0, 0);
		topLayout->addWidget(searchBar);
		topLayout->addWidget(btnSettings);
		topLayout->addWidget(lblFirmwareStatus);
		mainLayout->addLayout(topLayout);

		auto* toolbarLayout = new QHBoxLayout();
		toolbarLayout->setContentsMargins(0, 0, 0, 0);
		toolbarLayout->addWidget(btnAddGame);
		toolbarLayout->addWidget(btnOptimize);
		toolbarLayout->addWidget(btnPrebuildShaders);
		toolbarLayout->addWidget(btnControllerSetup);
		toolbarLayout->addWidget(btnUpdateAll);
		mainLayout->addLayout(toolbarLayout);

		mainLayout->addWidget(gameGrid);

		auto* detailsLayout = new QHBoxLayout();
		detailsLayout->setContentsMargins(0, 0, 0, 0);
		auto* infoLayout = new QVBoxLayout();
		infoLayout->setContentsMargins(0, 0, 0, 0);
		infoLayout->addWidget(lblGameTitle);
		infoLayout->addWidget(lblLastPlayed);
		infoLayout->addWidget(lblHoursPlayed);
		infoLayout->addWidget(lblStatus);
		detailsLayout->addLayout(infoLayout);
		detailsLayout->addStretch();
		detailsLayout->addWidget(btnPlay);
		mainLayout->addLayout(detailsLayout);

		setCentralWidget(central);

		connect(searchBar, &QLineEdit::textChanged, m_model, &ModernLibraryModel::setFilterString);
		connect(btnAddGame, &QPushButton::clicked, this, &ModernManagerWindow::slotAddGame);
		connect(btnOptimize, &QPushButton::clicked, this, &ModernManagerWindow::slotOptimize);
		connect(btnPrebuildShaders, &QPushButton::clicked, this, &ModernManagerWindow::slotPrebuildShaders);
		connect(btnControllerSetup, &QPushButton::clicked, this, &ModernManagerWindow::slotControllerSetup);
		connect(btnUpdateAll, &QPushButton::clicked, this, &ModernManagerWindow::slotUpdateAll);
		connect(gameGrid->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ModernManagerWindow::updateDetailsPane);
		connect(btnPlay, &QPushButton::clicked, this, &ModernManagerWindow::slotPlayGame);

		updateActions();
	}

	ModernManagerWindow::~ModernManagerWindow()
	{
		if (m_settings)
		{
			m_settings->SetValue("ui/modern_manager", "window_geometry", saveGeometry());
			m_settings->SetValue("ui/modern_manager", "show_favorites_only", m_show_favorites_only);
		}
	}

	ModernLibraryModel::GameEntry ModernManagerWindow::currentEntry() const
	{
		return m_model->entryAt(gameGrid->currentIndex());
	}

	void ModernManagerWindow::slotAddGame()
	{
		try
		{
			const QString dir = QFileDialog::getExistingDirectory(this, tr("Select Game Folder"));
			if (dir.isEmpty())
			{
				return;
			}

			if (Emu.AddGamesFromDir(dir.toStdString()) > 0)
			{
				m_model->refresh();
			}
		}
		catch (const std::exception& e)
		{
			modern_manager_log.error("AddGame failed: %s", e.what());
			QMessageBox box(QMessageBox::Critical, tr("Error"), tr("Failed to add game."), QMessageBox::Ok, this);
			auto* openLogs = box.addButton(tr("Open logs"), QMessageBox::ActionRole);
			box.exec();
			if (box.clickedButton() == openLogs)
			{
				QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(fs::get_log_dir())));
			}
		}
	}

	void ModernManagerWindow::slotOptimize()
	{
		const auto entry = currentEntry();
		if (entry.path.isEmpty())
		{
			return;
		}

		modern_manager_log.notice("Optimize requested for %s", entry.title_id.toStdString());
		ModernTelemetry::instance().logEvent("optimize_applied");
	}

	void ModernManagerWindow::slotPrebuildShaders()
	{
		const auto entry = currentEntry();
		if (entry.path.isEmpty())
		{
			return;
		}

		modern_manager_log.notice("Prebuild shaders requested for %s", entry.title_id.toStdString());
		ModernTelemetry::instance().logEvent("prebuild_start");
		ModernTelemetry::instance().logEvent("prebuild_finish");
	}

	void ModernManagerWindow::slotControllerSetup()
	{
		auto settings = std::make_shared<gui_settings>();
		pad_settings_dialog dlg(settings, this);
		dlg.exec();
	}

	void ModernManagerWindow::slotUpdateAll()
	{
		modern_manager_log.notice("Update all requested");
		m_model->refresh();
	}

	void ModernManagerWindow::slotPlayGame()
	{
		const auto entry = currentEntry();
		if (entry.path.isEmpty())
		{
			return;
		}

		ModernTelemetry::instance().logEvent("play_start");
		const auto res = Emu.BootGame(entry.path.toStdString(), entry.title_id.toStdString(), true);
		if (res != game_boot_result::no_errors)
		{
			modern_manager_log.error("Failed to boot %s", entry.title_id.toStdString());
			ModernTelemetry::instance().logEvent("play_stop", QStringLiteral("boot_failed"));
			QMessageBox box(QMessageBox::Critical, tr("Error"), tr("Failed to start game."), QMessageBox::Ok, this);
			auto* openLogs = box.addButton(tr("Open logs"), QMessageBox::ActionRole);
			box.exec();
			if (box.clickedButton() == openLogs)
			{
				QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(fs::get_log_dir())));
			}
		}
		else
		{
			ModernTelemetry::instance().logEvent("play_stop");
		}
	}

	void ModernManagerWindow::updateDetailsPane()
	{
		const auto entry = currentEntry();
		lblGameTitle->setText(entry.name);
		lblLastPlayed->setText(entry.last_played);
		lblHoursPlayed->setText(QString::number(entry.hours_played));
		lblStatus->setText(entry.status);
		updateActions();
	}

	void ModernManagerWindow::updateActions()
	{
		const bool has_game = !currentEntry().path.isEmpty();
		btnOptimize->setEnabled(has_game);
		btnPrebuildShaders->setEnabled(has_game);
		btnPlay->setEnabled(has_game);
	}
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
