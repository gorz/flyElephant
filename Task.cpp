#include "Task.hpp"

#include <QDebug>

Task::Task(const QJsonObject& object)
   : mId(object.value("id").toInt())
   , mState("Unknown state")
   , mName(object.value("taskName").toString())
   , mResult(object.value("resultName").toString())
   , mRepo(object.value("repository").toString())
   , mCommand(object.value("run_command").toString())
{

}

void Task::setId(int id)
{
   qDebug() << "id: " << id;
   mId = id;
   emit idChanged();
}

void Task::setState(QString state)
{
   qDebug() << "state: " << state;
   mState = state;
   emit stateChanged();
}

void Task::setName(QString name)
{
   qDebug() << "name: " << name;
   mName = name;
   emit nameChanged();
}

void Task::setResult(QString result)
{
   qDebug() << "result: " << result;
   mResult = result;
   emit resultChanged();
}

void Task::setRepo(QString repo)
{
   qDebug() << "repo: " << repo;
   mRepo = repo;
   emit repoChanged();
}

void Task::setCommand(QString command)
{
   qDebug() << "command: " << command;
   mCommand = command;
   emit commandChanged();
}

int Task::getId()
{
   return mId;
}

QString Task::getState()
{
   return mState;
}

QString Task::getName()
{
   return mName;
}

QString Task::getResult()
{
   return mResult;
}

QString Task::getRepo()
{
   return mRepo;
}

QString Task::getCommand()
{
   return mCommand;
}

namespace
{

std::map<QString, int> getSoftwares()
{
   std::map<QString, int> s;
   s["3"] = 0;
   s["4"] = 1;
   return s;
}

std::map<QString, int> getInstances()
{
   std::map<QString, int> s;
   s["A0"] = 0;
   s["A1"] = 1;
   return s;
}

}

int Task::getSoftware()
{
   static std::map<QString, int> s = getSoftwares();
   return s[mSoftware];
}

int Task::getInstance()
{
   static std::map<QString, int> i = getInstances();
   return i[mInstance];
}

