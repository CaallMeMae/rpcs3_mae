#pragma once

#ifdef ENABLE_MODERN_MANAGER

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QListView>
#include <memory>

#include "ModernLibraryModel.h"
#include "LibraryMetadataStore.h"

class gui_settings;

namespace rpcs3::ui
{
	class ModernManagerWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		explicit ModernManagerWindow(QWidget* parent = nullptr);
		~ModernManagerWindow() override;

	private Q_SLOTS:
		void slotAddGame();
		void slotOptimize();
		void slotPrebuildShaders();
		void slotControllerSetup();
		void slotUpdateAll();
		void slotPlayGame();
		void updateDetailsPane();
		void updateActions();

	private:
		ModernLibraryModel::GameEntry currentEntry() const;

		ModernLibraryModel* m_model = nullptr;
		std::shared_ptr<gui_settings> m_settings;
		LibraryMetadataStore m_metadata;
		bool m_show_favorites_only = false;

		QLineEdit* searchBar = nullptr;
		QPushButton* btnSettings = nullptr;
		QLabel* lblFirmwareStatus = nullptr;
		QPushButton* btnAddGame = nullptr;
		QPushButton* btnOptimize = nullptr;
		QPushButton* btnPrebuildShaders = nullptr;
		QPushButton* btnControllerSetup = nullptr;
		QPushButton* btnUpdateAll = nullptr;
		QListView* gameGrid = nullptr;
		QLabel* lblGameTitle = nullptr;
		QLabel* lblLastPlayed = nullptr;
		QLabel* lblHoursPlayed = nullptr;
		QLabel* lblStatus = nullptr;
		QPushButton* btnPlay = nullptr;
	};
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
