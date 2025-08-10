#ifdef ENABLE_MODERN_MANAGER

#include "LibraryMetadataStore.h"
#include "Utilities/File.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace rpcs3::ui
{
	LibraryMetadataStore::LibraryMetadataStore()
		: m_path(QString::fromStdString(fs::get_config_dir()) + "/modern_library_metadata.json")
	{
	}

	QJsonObject LibraryMetadataStore::load() const
	{
		if (!fs::is_file(m_path.toStdString()))
		{
			return {};
		}

		fs::file f(m_path.toStdString(), fs::read);
		if (!f)
		{
			return {};
		}

		std::string data;
		data.resize(f.size());
		f.read(data.data(), data.size());
		QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(data));
		if (doc.isObject())
		{
			return doc.object();
		}
		return {};
	}

	bool LibraryMetadataStore::save(const QJsonObject& obj) const
	{
		fs::pending_file pf(m_path.toStdString());
		if (!pf.file)
		{
			return false;
		}

		QJsonDocument doc(obj);
		QByteArray arr = doc.toJson(QJsonDocument::Compact);
		pf.file.write(arr.data(), arr.size());
		return pf.commit();
	}
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
