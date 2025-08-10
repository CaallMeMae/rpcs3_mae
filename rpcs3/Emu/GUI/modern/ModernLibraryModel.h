#pragma once

#include <QAbstractListModel>

namespace rpcs3::ui
{
	class ModernLibraryModel : public QAbstractListModel
	{
		Q_OBJECT

	public:
		explicit ModernLibraryModel(QObject* parent = nullptr);

		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	};
} // namespace rpcs3::ui
