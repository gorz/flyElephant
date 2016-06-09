#ifndef TASKLISTMODEL_HPP
#define TASKLISTMODEL_HPP

#include <QObject>
#include <QAbstractListModel>
#include "Task.hpp"
#include <QList>

class TaskListModel : public QAbstractListModel
{
   Q_OBJECT
public:
   explicit TaskListModel(QObject *parent = 0);

   enum
   {
      IdRole = Qt::UserRole + 1,
      NameRole,
      StateRole
   };

   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
   int rowCount(const QModelIndex &parent = QModelIndex()) const;
   QHash<int,QByteArray> roleNames() const;

   void add(Task* task);
   void remove(int i);
   void update(int i);
   const QList<Task*>& getTasks();

private:
   QList<Task*> mTasks;
};

#endif // TASKLISTMODEL_HPP
