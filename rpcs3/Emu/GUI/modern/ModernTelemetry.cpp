#ifdef ENABLE_MODERN_MANAGER

#include "ModernTelemetry.h"
#include "Utilities/File.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>

namespace rpcs3::ui
{
	ModernTelemetry& ModernTelemetry::instance()
	{
		static ModernTelemetry inst;
		return inst;
	}

	ModernTelemetry::ModernTelemetry()
		: m_path(QString::fromStdString(fs::get_config_dir()) + "/modern_manager_events.json")
	{
	}

	void ModernTelemetry::logEvent(const QString& name, const QString& reason)
	{
		QJsonArray arr;
		if (fs::is_file(m_path.toStdString()))
		{
			fs::file f(m_path.toStdString(), fs::read);
			if (f)
			{
				std::string data;
				data.resize(f.size());
				f.read(data.data(), data.size());
				QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(data));
				if (doc.isArray())
				{
					arr = doc.array();
				}
			}
		}

		QJsonObject obj;
		obj["event"] = name;
		obj["timestamp"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
		if (!reason.isEmpty())
		{
			obj["reason"] = reason;
		}
		arr.append(obj);
		while (arr.size() > 20)
		{
			arr.removeFirst();
		}

		fs::pending_file pf(m_path.toStdString());
		if (!pf.file)
		{
			return;
		}

		QJsonDocument out(arr);
		QByteArray bytes = out.toJson(QJsonDocument::Compact);
		pf.file.write(bytes.data(), bytes.size());
		pf.commit();
	}
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
