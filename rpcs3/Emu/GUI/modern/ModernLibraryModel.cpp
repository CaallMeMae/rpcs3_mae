#include "ModernLibraryModel.h"

#include "Emu/System.h"
#include "Emu/system_utils.hpp"
#include "Loader/PSF.h"
#include "Utilities/File.h"
#include "util/logs.hpp"

LOG_CHANNEL(modern_library_log, "ModernLibrary");

namespace rpcs3::ui
{
       ModernLibraryModel::ModernLibraryModel(QObject* parent)
               : QAbstractListModel(parent)
       {
               refresh();
       }

       int ModernLibraryModel::rowCount(const QModelIndex& parent) const
       {
               Q_UNUSED(parent);
               return static_cast<int>(m_filtered_indexes.size());
       }

       QVariant ModernLibraryModel::data(const QModelIndex& index, int role) const
       {
               if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
               {
                       return {};
               }

               const GameEntry& entry = m_games[m_filtered_indexes[index.row()]];

               switch (role)
               {
               case Qt::DisplayRole:
                       return entry.name;
               case Qt::DecorationRole:
                       return entry.icon;
               default:
                       return {};
               }
       }

       void ModernLibraryModel::setFilterString(const QString& filter)
       {
               if (m_filter == filter)
               {
                       return;
               }

               m_filter = filter;
               beginResetModel();
               applyFilter();
               endResetModel();
       }

       ModernLibraryModel::GameEntry ModernLibraryModel::entryAt(const QModelIndex& index) const
       {
               if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
               {
                       return {};
               }

               return m_games[m_filtered_indexes[index.row()]];
       }

       void ModernLibraryModel::refresh()
       {
               beginResetModel();
               m_games.clear();

               const auto game_map = Emu.GetGamesConfig().get_games();

               for (const auto& [title_id, path] : game_map)
               {
                       GameEntry entry;
                       entry.title_id = QString::fromStdString(title_id);
                       entry.path = QString::fromStdString(path);

                       const std::string sfo_dir = rpcs3::utils::get_sfo_dir_from_game_path(path, title_id);
                       const std::string sfo_path = sfo_dir + "/PARAM.SFO";
                       const psf::registry psf = psf::load_object(sfo_path);

                       std::string name = std::string(psf::get_string(psf, "TITLE"));
                       if (name.empty())
                       {
                               name = title_id;
                       }
                       entry.name = QString::fromStdString(name);

                       const QString icon_path = QString::fromStdString(sfo_dir + "/ICON0.PNG");
                       if (QFile::exists(icon_path))
                       {
                               entry.icon = QPixmap(icon_path);
                       }

                       entry.status = tr("Unknown");
                       entry.last_played = tr("Never");
                       entry.hours_played = 0;

                       m_games.emplace_back(std::move(entry));
               }

               applyFilter();
               endResetModel();
       }

       void ModernLibraryModel::applyFilter()
       {
               m_filtered_indexes.clear();

               for (int i = 0; i < static_cast<int>(m_games.size()); ++i)
               {
                       const auto& g = m_games[i];
                       if (m_filter.isEmpty() || g.name.contains(m_filter, Qt::CaseInsensitive) || g.title_id.contains(m_filter, Qt::CaseInsensitive))
                       {
                               m_filtered_indexes.push_back(i);
                       }
               }
       }
} // namespace rpcs3::ui
