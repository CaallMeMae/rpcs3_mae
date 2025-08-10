#include "ModernManagerWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QItemSelectionModel>
#include <memory>

#include "Emu/System.h"
#include "util/logs.hpp"
#include "rpcs3qt/pad_settings_dialog.h"
#include "rpcs3qt/gui_settings.h"

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
               btnOptimize = new QPushButton(tr("Optimize"), this);
               btnPrebuildShaders = new QPushButton(tr("Prebuild Shaders"), this);
               btnControllerSetup = new QPushButton(tr("Controller Setup"), this);
               btnUpdateAll = new QPushButton(tr("Update All"), this);

               gameGrid = new QListView(this);
               gameGrid->setViewMode(QListView::IconMode);
               gameGrid->setWrapping(true);
               gameGrid->setResizeMode(QListView::Adjust);

               lblGameTitle = new QLabel(this);
               lblLastPlayed = new QLabel(this);
               lblHoursPlayed = new QLabel(this);
               lblStatus = new QLabel(this);
               btnPlay = new QPushButton(tr("Play"), this);

               m_model = new ModernLibraryModel(this);
               gameGrid->setModel(m_model);

               auto* central = new QWidget(this);
               auto* mainLayout = new QVBoxLayout(central);

               auto* topLayout = new QHBoxLayout();
               topLayout->addWidget(searchBar);
               topLayout->addWidget(btnSettings);
               topLayout->addWidget(lblFirmwareStatus);
               mainLayout->addLayout(topLayout);

               auto* toolbarLayout = new QHBoxLayout();
               toolbarLayout->addWidget(btnAddGame);
               toolbarLayout->addWidget(btnOptimize);
               toolbarLayout->addWidget(btnPrebuildShaders);
               toolbarLayout->addWidget(btnControllerSetup);
               toolbarLayout->addWidget(btnUpdateAll);
               mainLayout->addLayout(toolbarLayout);

               mainLayout->addWidget(gameGrid);

               auto* detailsLayout = new QHBoxLayout();
               auto* infoLayout = new QVBoxLayout();
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
       }

       ModernManagerWindow::~ModernManagerWindow() = default;

       ModernLibraryModel::GameEntry ModernManagerWindow::currentEntry() const
       {
               return m_model->entryAt(gameGrid->currentIndex());
       }

       void ModernManagerWindow::slotAddGame()
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

       void ModernManagerWindow::slotOptimize()
       {
               const auto entry = currentEntry();
               if (!entry.title_id.isEmpty())
               {
                       modern_manager_log.notice("Optimize requested for %s", entry.title_id.toStdString());
               }
       }

       void ModernManagerWindow::slotPrebuildShaders()
       {
               const auto entry = currentEntry();
               if (!entry.title_id.isEmpty())
               {
                       modern_manager_log.notice("Prebuild shaders requested for %s", entry.title_id.toStdString());
               }
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

               const auto res = Emu.BootGame(entry.path.toStdString(), entry.title_id.toStdString(), true);
               if (res != game_boot_result::no_errors)
               {
                       modern_manager_log.error("Failed to boot %s", entry.title_id.toStdString());
               }
       }

       void ModernManagerWindow::updateDetailsPane()
       {
               const auto entry = currentEntry();
               lblGameTitle->setText(entry.name);
               lblLastPlayed->setText(entry.last_played);
               lblHoursPlayed->setText(QString::number(entry.hours_played));
               lblStatus->setText(entry.status);
       }
} // namespace rpcs3::ui
