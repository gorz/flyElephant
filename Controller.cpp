#include "Controller.hpp"

#include <iostream>

#include <QDebug>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QFile>
#include <QJsonArray>

Controller::Controller(QObject *parent)
   : QObject(parent)
   , mNetwork(new QNetworkAccessManager(parent))
   , mSettings(loadJson("data.json").object())
{
   QObject::connect(mNetwork, SIGNAL(finished(QNetworkReply *)), this, SLOT(finish(QNetworkReply *)));
   setApiKey("5ucTO9zQf4gCjyJc");

   QJsonArray tasks = mSettings.value("tasks").toArray();
   for(int i = 0; i < tasks.size(); i++)
   {
      mTasks.add(new Task(tasks.at(i).toObject()));
   }
   refreshTask();
}

void Controller::setApiKey(QString key)
{
   qDebug() << "new api key" << key;
   mKey = key;
   mSettings.insert("key", key);
   saveJson(mSettings, "data.json");
}

void Controller::createTask(QString taskName, QString resultName, QString repo,
                            QString software, QString instance, QString command)
{
   qDebug() << "create task" << taskName << " " << resultName << " "
            << repo << " " << software << " " << instance << " " << command;

   QJsonObject taskData;
   taskData.insert("taskName", taskName);
   taskData.insert("resultName", resultName);
   taskData.insert("repository", repo);
   taskData.insert("software_id", software);
   taskData.insert("instance", instance);
   taskData.insert("run_command", command);
   taskData.insert("account_type", "2");

   QUrlQuery postData;
   postData.addQueryItem("taskName", taskName);
   postData.addQueryItem("resultName", resultName);
   postData.addQueryItem("repository", repo);
   postData.addQueryItem("software_id", software);
   postData.addQueryItem("instance", instance);
   postData.addQueryItem("run_command", command);
   postData.addQueryItem("account_type", "2");

   QNetworkRequest request;
   request.setAttribute(QNetworkRequest::UserMax, QVariant(Create));
   request.setAttribute(QNetworkRequest::User, QVariant(taskData));
   request.setUrl(QUrl("https://api.flyelephant.net/v1/tasks/initiateTask"));
   request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
   QString key("api-key");
   request.setRawHeader(key.toUtf8(), mKey.toUtf8());

   qDebug() << postData.toString(QUrl::FullyEncoded);
   mNetwork->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
}

void Controller::refreshTask()
{
   const QList<Task*>& tasks = mTasks.getTasks();
   for(int i = 0; i < tasks.size(); i++)
   {
      QNetworkRequest request;
      request.setAttribute(QNetworkRequest::User, QVariant(i));
      request.setAttribute(QNetworkRequest::UserMax, QVariant(Refresh));
      request.setUrl(QUrl("https://api.flyelephant.net/v1/tasks/" + QVariant(tasks[i]->getId()).toString() + "/getStatus"));
      QString key("api-key");
      request.setRawHeader(key.toUtf8(), mKey.toUtf8());

      mNetwork->get(request);
   }
}

void Controller::deleteTask(int i)
{
   const QList<Task*>& tasks = mTasks.getTasks();
   QNetworkRequest request;
   request.setAttribute(QNetworkRequest::User, QVariant(i));
   request.setAttribute(QNetworkRequest::UserMax, QVariant(Delete));
   request.setUrl(QUrl("https://api.flyelephant.net/v1/tasks/" + QVariant(tasks[i]->getId()).toString() + "/delete"));
   QString key("api-key");
   request.setRawHeader(key.toUtf8(), mKey.toUtf8());
   mNetwork->deleteResource(request);
}

void Controller::stopTask(int i)
{
   const QList<Task*>& tasks = mTasks.getTasks();
   QNetworkRequest request;
   request.setAttribute(QNetworkRequest::User, QVariant(i));
   request.setAttribute(QNetworkRequest::UserMax, QVariant(Stop));
   request.setUrl(QUrl("https://api.flyelephant.net/v1/tasks/" + QVariant(tasks[i]->getId()).toString() + "/stop"));
   QString key("api-key");
   request.setRawHeader(key.toUtf8(), mKey.toUtf8());

   QByteArray data;
   mNetwork->post(request, data);
}

void Controller::finish(QNetworkReply *reply)
{
   const int type = reply->request().attribute(QNetworkRequest::UserMax).toInt();
   if(reply->error() > 0)
   {
      qDebug() << reply->errorString();
   }
   else if(type == Refresh)
   {
      const QList<Task*>& tasks = mTasks.getTasks();
      int i = reply->request().attribute(QNetworkRequest::User).toInt();
      Task* task = tasks[i];

      QByteArray result = reply->readAll();
      std::cout << result.toStdString() << std::endl;
      QJsonDocument doc = QJsonDocument::fromJson(result);
      task->setState(doc.object().value("taskStatus").toString("Refresh field"));
      mTasks.update(i);
   }
   else if(type == Create)
   {
      QByteArray result = reply->readAll();
      QJsonDocument doc = QJsonDocument::fromJson(result);
      std::cout << result.toStdString() << std::endl;

      QJsonObject taskData = reply->request().attribute(QNetworkRequest::User).toJsonObject();
      taskData.insert("id", doc.object().value("id"));

      QJsonArray array = mSettings.value("tasks").toArray();
      array.append(taskData);
      mSettings.insert("tasks", array);
      saveJson(mSettings, "data.json");

      Task* task = new Task(taskData);
      task->setState("Created");
      mTasks.add(task);
   }
   else if(type == Delete)
   {
      int i = reply->request().attribute(QNetworkRequest::User).toInt();
      mTasks.remove(i);
   }
   else if(type == Stop)
   {
      const QList<Task*>& tasks = mTasks.getTasks();
      int i = reply->request().attribute(QNetworkRequest::User).toInt();
      Task* task = tasks[i];

      task->setState("Stopped");
      mTasks.update(i);
   }
   else if(type == Result)
   {
      QNetworkRequest request;
      request.setAttribute(QNetworkRequest::User, reply->request().attribute(QNetworkRequest::User));
      request.setAttribute(QNetworkRequest::UserMax, QVariant(Download));

      QByteArray result = reply->readAll();
      std::cout << result.toStdString() << std::endl;
      QJsonDocument doc = QJsonDocument::fromJson(result);
      request.setUrl(doc.object().value("result").toString());

      QString key("api-key");
      request.setRawHeader(key.toUtf8(), mKey.toUtf8());

      mNetwork->get(request);
   }
   else if(type == Download)
   {
      QString result = reply->request().attribute(QNetworkRequest::User).toString();
      QFile file(result);
      file.open(QFile::WriteOnly);
      file.write(reply->readAll());
   }
}

TaskListModel* Controller::getTasks()
{
   return &mTasks;
}

QObject* Controller::getTask(int i)
{
   if (i >= mTasks.getTasks().size())
   {
      return 0;
   }
   return mTasks.getTasks()[i];
}

void Controller::downloadResult(int i)
{
   if (i >= mTasks.getTasks().size())
   {
      return;
   }
   Task* task = mTasks.getTasks()[i];
   QNetworkRequest request;
   request.setAttribute(QNetworkRequest::User, QVariant(task->getResult()));
   request.setAttribute(QNetworkRequest::UserMax, QVariant(Result));
   request.setUrl(QUrl("https://api.flyelephant.net/v1/tasks/" + QVariant(task->getId()).toString() + "/result"));
   QString key("api-key");
   request.setRawHeader(key.toUtf8(), mKey.toUtf8());

   mNetwork->get(request);
}

QJsonDocument Controller::loadJson(QString fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument::fromJson(jsonFile.readAll());
}

void Controller::saveJson(QJsonObject obj, QString fileName)
{
   QJsonDocument document;
   document.setObject(obj);
   QFile jsonFile(fileName);
   jsonFile.open(QFile::WriteOnly);
   jsonFile.write(document.toJson());
}

