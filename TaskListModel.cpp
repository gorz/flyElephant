#include "TaskListModel.hpp"

TaskListModel::TaskListModel(QObject *parent)
{

}

QHash<int, QByteArray> TaskListModel::roleNames() const
{
   QHash<int, QByteArray> hash({
      {IdRole, QString("id").toUtf8()},
      {NameRole, QString("name").toUtf8()},
      {StateRole, QString("state").toUtf8()}});
   return hash;
}

int TaskListModel::rowCount(const QModelIndex &parent) const
{
   return mTasks.size();
}

QVariant TaskListModel::data(const QModelIndex &index, int role) const
{
   if (index.row() >= mTasks.size())
   {
      return QVariant();
   }

   Task* task = mTasks[index.row()];
   switch (role)
   {
   case IdRole:
      return QVariant(task->getId());
   case NameRole:
      return QVariant(task->getName());
   case StateRole:
      return QVariant(task->getState());
   }

   return QVariant();
}

void TaskListModel::add(Task *task)
{
   beginInsertRows(QModelIndex(), mTasks.size(), mTasks.size());
   mTasks.push_back(task);
   endInsertRows();
}

void TaskListModel::remove(int i)
{
   beginRemoveRows(QModelIndex(), i, i);
   mTasks.removeAt(i);
   endRemoveRows();
}

void TaskListModel::update(int i)
{
   emit dataChanged(index(i), index(i));
}

const QList<Task*>& TaskListModel::getTasks()
{
   return mTasks;
}

