#pragma once
#ifdef ENABLE_MODERN_MANAGER

#include <QJsonObject>
#include <QString>

namespace rpcs3::ui
{
	class LibraryMetadataStore
	{
	public:
		LibraryMetadataStore();
		QJsonObject load() const;
		bool save(const QJsonObject& obj) const;

	private:
		QString m_path;
	};
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
