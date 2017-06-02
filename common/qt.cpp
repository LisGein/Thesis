#include "common.h"
#include "qt.h"


QString to_qt(const std::string& str)
{
	return QString::fromStdString(str);
}

QStringList to_qt(const std::vector<std::string>& stringList)
{
	QStringList res;
	for (const auto& str : stringList)
	{
		res << to_qt(str);
	}

	return res;
}



std::vector<std::string> from_qt(const QStringList& stringList)
{
	std::vector<std::string> res;
	for (const auto& str : stringList)
	{
		res.push_back(str.toStdString());
	}

	return res;
}
