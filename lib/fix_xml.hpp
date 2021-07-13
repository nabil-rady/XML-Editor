#pragma once
#include <QStack>
#include <QString>
#include "check.hpp"

QString fix_xml(QString xml);
bool check_with_counting(QString file, int* startindex, int* endindex, int* openingtag, int* closingtag, int* selfclosingtag,QString* expected);

