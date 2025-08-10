#pragma once
#ifdef ENABLE_MODERN_MANAGER

#include <QString>

namespace rpcs3::ui
{
	class ModernTelemetry
	{
	public:
		static ModernTelemetry& instance();
		void logEvent(const QString& name, const QString& reason = {});

	private:
		ModernTelemetry();
		QString m_path;
	};
} // namespace rpcs3::ui

#endif // ENABLE_MODERN_MANAGER
