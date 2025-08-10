#include "ModernLibraryModel.h"

namespace rpcs3::ui
{
	ModernLibraryModel::ModernLibraryModel(QObject* parent)
		: QAbstractListModel(parent)
	{
	}

	int ModernLibraryModel::rowCount(const QModelIndex& parent) const
	{
		Q_UNUSED(parent);
		return 0;
	}

	QVariant ModernLibraryModel::data(const QModelIndex& index, int role) const
	{
		Q_UNUSED(index);
		Q_UNUSED(role);
		return {};
	}
} // namespace rpcs3::ui
