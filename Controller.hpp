#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QJsonObject>

#include "Task.hpp"

#include "TaskListModel.hpp"

class Controller : public QObject
{
   Q_OBJECT
public:
   explicit Controller(QObject *parent = 0);

   Q_INVOKABLE void setApiKey(QString key);
   Q_INVOKABLE void createTask(QString taskName, QString resultName, QString repo,
                               QString software, QString instance, QString command);
   Q_INVOKABLE void refreshTask();
   Q_INVOKABLE void deleteTask(int i);
   Q_INVOKABLE void stopTask(int i);
   Q_INVOKABLE QObject* getTask(int i);
   Q_INVOKABLE void downloadResult(int i);

   TaskListModel* getTasks();

public slots:
   void finish(QNetworkReply *reply);

private:
   enum Request
   {
      Create,
      Refresh,
      Stop,
      Delete,
      Result,
      Download
   };

   QJsonDocument loadJson(QString fileName);
   void saveJson(QJsonObject obj, QString fileName);

   QString mKey;
   QNetworkAccessManager* mNetwork;
   TaskListModel mTasks;
   QJsonObject mSettings;
};

#endif // CONTROLLER_HPP
