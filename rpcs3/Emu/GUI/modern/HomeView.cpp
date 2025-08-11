#ifdef ENABLE_MODERN_MANAGER

#include "HomeView.h"
#include "BigTile.h"
#include "ModernLibraryModel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

namespace rpcs3::ui
{
	HomeView::HomeView(QWidget* parent)
		: QWidget(parent)
	{
		auto* layout = new QVBoxLayout(this);
		layout->setContentsMargins(8, 8, 8, 8);
		layout->setSpacing(8);

		m_continue = new BigTile(this);
		m_continue->setTitle(tr("Continue"));
		layout->addWidget(m_continue);
		connect(m_continue, &BigTile::activated, [this]()
			{
				if (!m_model)
					return;
				// use first entry as last played
				if (m_model->rowCount() > 0)
				{
					auto entry = m_model->entryAt(m_model->index(0, 0));
					Q_EMIT playRequested(entry.path);
				}
			});

		m_recentsWidget = new QWidget(this);
		auto* recentsLayout = new QHBoxLayout(m_recentsWidget);
		recentsLayout->setContentsMargins(0, 0, 0, 0);
		recentsLayout->setSpacing(8);
		layout->addWidget(m_recentsWidget);

		QLabel* friends = new QLabel(tr("Friends Online: 0"), this);
		layout->addWidget(friends);

		m_actionsWidget = new QWidget(this);
		auto* actionsLayout = new QHBoxLayout(m_actionsWidget);
		actionsLayout->setContentsMargins(0, 0, 0, 0);
		actionsLayout->setSpacing(8);
		layout->addWidget(m_actionsWidget);

		// Quick actions
		BigTile* installPkg = new BigTile(this);
		installPkg->setTitle(tr("Install .pkg"));
		connect(installPkg, &BigTile::activated, this, &HomeView::installPkgRequested);
		actionsLayout->addWidget(installPkg);

		BigTile* addFolder = new BigTile(this);
		addFolder->setTitle(tr("Add Disc Folder"));
		connect(addFolder, &BigTile::activated, this, &HomeView::addDiscFolderRequested);
		actionsLayout->addWidget(addFolder);

		BigTile* settings = new BigTile(this);
		settings->setTitle(tr("Settings"));
		connect(settings, &BigTile::activated, this, &HomeView::settingsRequested);
		actionsLayout->addWidget(settings);

		BigTile* online = new BigTile(this);
		online->setTitle(tr("Online"));
		connect(online, &BigTile::activated, this, &HomeView::onlineRequested);
		actionsLayout->addWidget(online);
	}

	void HomeView::setModel(ModernLibraryModel* m)
	{
		m_model = m;
		rebuild();
	}

	void HomeView::setSocialService(ISocialService* s)
	{
		m_social = s;
		Q_UNUSED(m_social);
	}

	void HomeView::rebuild()
	{
		if (!m_model)
			return;
		// Clear recents
		delete m_recentsWidget;
		m_recentsWidget = new QWidget(this);
		auto* recentsLayout = new QHBoxLayout(m_recentsWidget);
		recentsLayout->setContentsMargins(0, 0, 0, 0);
		recentsLayout->setSpacing(8);
		layout()->addWidget(m_recentsWidget);

		const int count = std::min(12, m_model->rowCount());
		for (int i = 0; i < count; ++i)
		{
			auto entry = m_model->entryAt(m_model->index(i, 0));
			BigTile* tile = new BigTile(this);
			tile->setTitle(entry.name);
			tile->setPixmap(entry.icon);
			connect(tile, &BigTile::activated, this, [this, entry]()
				{
					Q_EMIT openDetailsRequested(entry.title_id);
				});
			recentsLayout->addWidget(tile);
		}
	}
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
