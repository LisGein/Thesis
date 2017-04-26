#pragma once

#include <QString>
#include <QStringList>
#include <QPair>

QString to_qt(const std::string& str);

QStringList to_qt(const std::vector<std::string>& stringList);

std::vector<std::string> from_qt(const QStringList& stringList);

template <typename T, typename U>
std::pair<T, U> from_qt(const QPair<T, U>& pair){ return std::make_pair(pair.first, pair.second); }
