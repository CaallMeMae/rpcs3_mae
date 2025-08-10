#pragma once

#include <QAbstractListModel>
#include <QPixmap>
#include <vector>

struct GameInfo; // forward decl if needed? But we will use psf not GameInfo. but not needed

namespace rpcs3::ui
{
       class ModernLibraryModel : public QAbstractListModel
       {
               Q_OBJECT

       public:
               struct GameEntry
               {
                       QString title_id;
                       QString name;
                       QString path;
                       QPixmap icon;
                       QString status;
                       QString last_played;
                       quint64 hours_played = 0;
               };

               explicit ModernLibraryModel(QObject* parent = nullptr);

               int rowCount(const QModelIndex& parent = QModelIndex()) const override;
               QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

               void setFilterString(const QString& filter);
               GameEntry entryAt(const QModelIndex& index) const;
               void refresh();

       private:
               void applyFilter();

               QString m_filter;
               std::vector<GameEntry> m_games;
               std::vector<int> m_filtered_indexes;
       };
} // namespace rpcs3::ui
