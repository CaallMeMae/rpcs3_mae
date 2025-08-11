#ifdef ENABLE_MODERN_MANAGER

#include "ModernManagerWindow.h"
#include "HomeView.h"
#include "FocusNav.h"
#include "ModernSettingsPanel.h"
#include "ModernTelemetry.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QItemSelectionModel>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QShortcut>
#include <QApplication>
#include <QKeyEvent>

#include "Emu/System.h"
#include "Utilities/File.h"
#include "util/logs.hpp"
#include "rpcs3qt/pad_settings_dialog.h"
#include "rpcs3qt/gui_settings.h"

LOG_CHANNEL(modern_manager_log, "ModernManager");

namespace rpcs3::ui
{
	ModernManagerWindow::ModernManagerWindow(QWidget* parent)
		: QMainWindow(parent)
	{
		m_settings = std::make_shared<gui_settings>();
		if (m_settings->GetValue("modern", "large_text", false).toBool())
		{
			QFont f = QApplication::font();
			f.setPointSize(f.pointSize() + 2);
			QApplication::setFont(f);
		}

		navHome = new QPushButton(tr("Home"), this);
		navLibrary = new QPushButton(tr("Library"), this);
		navHome->setCheckable(true);
		navLibrary->setCheckable(true);
		navHome->setFocusPolicy(Qt::StrongFocus);
		navLibrary->setFocusPolicy(Qt::StrongFocus);
		connect(navHome, &QPushButton::clicked, this, &ModernManagerWindow::showHome);
		connect(navLibrary, &QPushButton::clicked, this, &ModernManagerWindow::showLibrary);

		m_pages = new QStackedWidget(this);

		// Models
		m_model = new ModernLibraryModel(this);

		// Home page
		m_home = new HomeView(this);
		m_home->setModel(m_model);
		m_pages->addWidget(m_home);
		connect(m_home, &HomeView::playRequested, this, &ModernManagerWindow::requestBootGame);
		connect(m_home, &HomeView::openDetailsRequested, this, [this](const QString& id)
			{
				showLibrary();
				for (int row = 0; row < m_model->rowCount(); ++row)
				{
					auto entry = m_model->entryAt(m_model->index(row, 0));
					if (entry.title_id == id)
					{
						gameGrid->setCurrentIndex(m_model->index(row, 0));
						break;
					}
				}
			});

		// Library page

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
		gameGrid->setFocusPolicy(Qt::StrongFocus);
		gameGrid->setModel(m_model);

		lblGameTitle = new QLabel(this);
		lblLastPlayed = new QLabel(this);
		lblHoursPlayed = new QLabel(this);
		lblStatus = new QLabel(this);
		btnPlay = new QPushButton(tr("Play"), this);
		btnPlay->setToolTip(tr("Boot selected game"));

		m_libraryPage = new QWidget(this);
		auto* mainLayout = new QVBoxLayout(m_libraryPage);
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

		m_pages->addWidget(m_libraryPage);

		auto* root = new QWidget(this);
		auto* rootLayout = new QVBoxLayout(root);
		auto* navLayout = new QHBoxLayout();
		navLayout->addWidget(navHome);
		navLayout->addWidget(navLibrary);
		rootLayout->addLayout(navLayout);
		rootLayout->addWidget(m_pages);
		setCentralWidget(root);

		connect(searchBar, &QLineEdit::textChanged, m_model, &ModernLibraryModel::setFilterString);
		connect(btnAddGame, &QPushButton::clicked, this, &ModernManagerWindow::slotAddGame);
		connect(btnOptimize, &QPushButton::clicked, this, &ModernManagerWindow::slotOptimize);
		connect(btnPrebuildShaders, &QPushButton::clicked, this, &ModernManagerWindow::slotPrebuildShaders);
		connect(btnControllerSetup, &QPushButton::clicked, this, &ModernManagerWindow::slotControllerSetup);
		connect(btnUpdateAll, &QPushButton::clicked, this, &ModernManagerWindow::slotUpdateAll);
		connect(gameGrid->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ModernManagerWindow::updateDetailsPane);
		connect(btnPlay, &QPushButton::clicked, this, &ModernManagerWindow::slotPlayGame);
		connect(btnSettings, &QPushButton::clicked, this, &ModernManagerWindow::openSettings);

		connect(m_home, &HomeView::installPkgRequested, this, &ModernManagerWindow::slotAddGame);
		connect(m_home, &HomeView::addDiscFolderRequested, this, &ModernManagerWindow::slotAddGame);
		connect(m_home, &HomeView::settingsRequested, this, &ModernManagerWindow::openSettings);

		new QShortcut(QKeySequence(Qt::Key_F11), this, [this]()
			{
				isFullScreen() ? showNormal() : showFullScreen();
			});

		FocusNav::install(m_home);
		FocusNav::install(m_libraryPage);
		showHome();
		updateActions();
	}

	ModernManagerWindow::~ModernManagerWindow()
	{
		if (m_settings)
		{
			m_settings->SetValue("ui/modern_manager", "window_geometry", saveGeometry());
		}
	}

	void ModernManagerWindow::requestBootGame(const QString& path)
	{
		if (path.isEmpty())
			return;
		ModernTelemetry::instance().logEvent("last_launched_from", "home");
		ModernTelemetry::instance().logEvent("play_start");
		const auto res = Emu.BootGame(path.toStdString(), std::string{}, true);
		if (res != game_boot_result::no_errors)
		{
			modern_manager_log.error("Failed to boot %s", path.toStdString());
			QMessageBox box(QMessageBox::Critical, tr("Error"), tr("Failed to start game."), QMessageBox::Ok, this);
			auto* openLogs = box.addButton(tr("Open logs"), QMessageBox::ActionRole);
			box.exec();
			if (box.clickedButton() == openLogs)
			{
				QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(fs::get_log_dir())));
			}
		}
	}

	void ModernManagerWindow::slotAddGame()
	{
		const QString dir = QFileDialog::getExistingDirectory(this, tr("Select Game Folder"));
		if (dir.isEmpty())
			return;
		if (Emu.AddGamesFromDir(dir.toStdString()) > 0)
			m_model->refresh();
	}

	void ModernManagerWindow::slotOptimize()
	{ /* stub */
	}
	void ModernManagerWindow::slotPrebuildShaders()
	{ /* stub */
	}
	void ModernManagerWindow::slotControllerSetup()
	{
		auto settings = std::make_shared<gui_settings>();
		pad_settings_dialog dlg(settings, this);
		dlg.exec();
	}

	void ModernManagerWindow::slotUpdateAll()
	{
		m_model->refresh();
	}

	void ModernManagerWindow::slotPlayGame()
	{
		const auto entry = currentEntry();
		if (entry.path.isEmpty())
			return;
		ModernTelemetry::instance().logEvent("play_start");
		const auto res = Emu.BootGame(entry.path.toStdString(), entry.title_id.toStdString(), true);
		if (res != game_boot_result::no_errors)
		{
			modern_manager_log.error("Failed to boot %s", entry.title_id.toStdString());
			QMessageBox box(QMessageBox::Critical, tr("Error"), tr("Failed to start game."), QMessageBox::Ok, this);
			auto* openLogs = box.addButton(tr("Open logs"), QMessageBox::ActionRole);
			box.exec();
			if (box.clickedButton() == openLogs)
				QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(fs::get_log_dir())));
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

	void ModernManagerWindow::showHome()
	{
		m_pages->setCurrentWidget(m_home);
		navHome->setChecked(true);
		navLibrary->setChecked(false);
	}

	void ModernManagerWindow::showLibrary()
	{
		m_pages->setCurrentWidget(m_libraryPage);
		navHome->setChecked(false);
		navLibrary->setChecked(true);
	}

	void ModernManagerWindow::openSettings()
	{
		if (!m_settings_panel)
		{
			m_settings_panel = new ModernSettingsPanel(m_settings, this);
		}
		m_settings_panel->exec();
	}

	ModernLibraryModel::GameEntry ModernManagerWindow::currentEntry() const
	{
		return m_model->entryAt(gameGrid->currentIndex());
	}

	void ModernManagerWindow::keyPressEvent(QKeyEvent* event)
	{
		if ((event->key() == Qt::Key_Escape || event->key() == Qt::Key_Backspace) && m_pages->currentWidget() == m_libraryPage)
		{
			showHome();
			return;
		}
		QMainWindow::keyPressEvent(event);
	}
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
