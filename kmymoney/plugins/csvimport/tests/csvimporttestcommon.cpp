/***************************************************************************
                        csvimporttestcommon.cpp
                      -------------------
copyright            : (C) 2017 by Łukasz Wojniłowicz
email                : lukasz.wojnilowicz@gmail.com
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#include "csvimporttestcommon.h"

#include <QFile>
#include <QTextStream>

#include "mymoneyfile.h"

void writeStatementToCSV(const QString& content, const QString& filename)
{
  QFile g(filename);
  g.open(QIODevice::WriteOnly);
  QTextStream stream (&g);
  stream << content;
  g.close();
}

QString csvDataset(const int set) {
  QString csvContent;
  switch (set) {
    case 0:
      csvContent += QLatin1String("Date;Name;Type;Quantity;Price;Amount;Fee\n");
      csvContent += QLatin1String("2017-08-01-12.02.10;Stock 1;buy;100;1.25;125;4\n");  // positive amount here is not good, but KMM can hadle it
      csvContent += QLatin1String("2017-08-02-12.02.10;Stock 2;sell;100;4.56;456;6\n");
      csvContent += QLatin1String("2017-08-03-12.02.10;Stock 3;buy;200;5.67;1134;4\n");
      break;
    default:
      break;
  }
  return csvContent;
}

QString makeAccount(const QString& name, const QString& number, MyMoneyAccount::accountTypeE type, const QDate& open, const QString& parent)
{
  MyMoneyAccount acc;
  MyMoneyFileTransaction ft;
  auto file = MyMoneyFile::instance();

  acc.setName(name);
  acc.setNumber(number);
  acc.setAccountType(type);
  acc.setOpeningDate(open);
  acc.setCurrencyId(file->baseCurrency().id());

  auto parentAcc = file->account(parent);
  file->addAccount(acc, parentAcc);
  ft.commit();

  return acc.id();
}