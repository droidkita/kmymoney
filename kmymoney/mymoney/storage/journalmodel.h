/*
 * Copyright 2019       Thomas Baumgart <tbaumgart@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JOURNALMODEL_H
#define JOURNALMODEL_H

// ----------------------------------------------------------------------------
// QT Includes

#include <QSharedDataPointer>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoneymodel.h"
#include "mymoneyenums.h"
#include "kmm_mymoney_export.h"

#include "mymoneyobject.h"
#include "mymoneytransaction.h"
#include "mymoneysplit.h"

class MyMoneyTransactionFilter;

/**
 * The class representing a single journal entry (one split of a transaction)
 */
class /* no export here on purpose */ JournalEntry
{
public:
  explicit JournalEntry() {}
  JournalEntry(QString id, QSharedPointer<MyMoneyTransaction> t, const MyMoneySplit& sp)
  : m_id(id)
  , m_transaction(t)
  , m_split(sp)
  {}

  inline const MyMoneyTransaction& transaction() const { return *m_transaction; }
  inline const MyMoneySplit& split() const { return m_split; }
  inline const QString& id() const { return m_id; }

private:
  QString                             m_id;
  QSharedPointer<MyMoneyTransaction>  m_transaction;
  MyMoneySplit                        m_split;

};

class JournalModelNewTransaction;

/**
  */
class KMM_MYMONEY_EXPORT JournalModel : public MyMoneyModel<JournalEntry>
{
  Q_OBJECT

public:
  enum Column {
    Number = 0,
    Date,
    Security,
    CostCenter,
    Detail,
    Reconciliation,
    Payment,
    Deposit,
    Quantity,
    Price,
    Amount,
    Value,
    Balance,
    // insert new columns above this line
    MaxColumns
  };

  explicit JournalModel(QObject* parent = nullptr);
  virtual ~JournalModel();

  static const int ID_SIZE = 18;

  int columnCount(const QModelIndex& parent = QModelIndex()) const final override;
  QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const final override;

  /**
   * Special implementation using a binary search algorithm instead
   * of the linear one provided by the template function
   */
  MyMoneyTransaction transactionById(const QString& id) const;

  void addTransaction(MyMoneyTransaction& t);
  void removeTransaction(const MyMoneyTransaction& t);
  void modifyTransaction(const MyMoneyTransaction& newTransaction);

  void transactionList(QList<MyMoneyTransaction>& list, MyMoneyTransactionFilter& filter) const;
  void transactionList(QList< QPair<MyMoneyTransaction, MyMoneySplit> >& list, MyMoneyTransactionFilter& filter) const;

  bool setData(const QModelIndex& idx, const QVariant& value, int role = Qt::EditRole) override;

  void load(const QMap<QString, MyMoneyTransaction>& list);

  JournalModelNewTransaction* newTransaction();

private:
  void addTransaction(const QString& id, MyMoneyTransaction& t);
  void removeTransaction(const QModelIndex& idx);

  QModelIndex firstIndexById(const QString& id) const;
  QModelIndex firstIndexByKey(const QString& key) const;

  void updateBalances();

Q_SIGNALS:
  void balancesChanged(const QHash<QString, MyMoneyMoney>& balances) const;

public Q_SLOTS:

private:
  struct Private;
  QScopedPointer<Private> d;
};

class KMM_MYMONEY_EXPORT JournalModelNewTransaction : public JournalModel
{
  Q_OBJECT

public:
  explicit JournalModelNewTransaction(QObject* parent = nullptr);
  virtual ~JournalModelNewTransaction();

  QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const final override;

protected:
  void addTransaction(MyMoneyTransaction& t) { Q_UNUSED(t); }
  void removeTransaction(const MyMoneyTransaction& t) { Q_UNUSED(t); }
  void modifyTransaction(const MyMoneyTransaction& newTransaction) { Q_UNUSED(newTransaction); }

  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) final override
  {
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    return false;
  }

  void load(const QMap<QString, MyMoneyTransaction>& list);
};
#endif // JOURNALMODEL_H
