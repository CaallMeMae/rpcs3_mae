#pragma once
#ifdef ENABLE_MODERN_MANAGER

#include <QWidget>
#include <QVector>

namespace rpcs3::ui
{
	class ModernLibraryModel;
	class ISocialService;
	class BigTile;

	class HomeView : public QWidget
	{
		Q_OBJECT
	public:
		explicit HomeView(QWidget* parent = nullptr);

		void setModel(ModernLibraryModel* m);
		void setSocialService(ISocialService* s);

	Q_SIGNALS:
		void playRequested(QString path);
		void openDetailsRequested(QString title_id);
		void installPkgRequested();
		void addDiscFolderRequested();
		void settingsRequested();
		void onlineRequested();

	private:
		void rebuild();
		ModernLibraryModel* m_model = nullptr;
		ISocialService* m_social = nullptr;
		BigTile* m_continue = nullptr;
		QWidget* m_recentsWidget = nullptr;
		QWidget* m_actionsWidget = nullptr;
	};
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
