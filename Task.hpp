#ifndef TASK_HPP
#define TASK_HPP

#include <QObject>
#include <QJsonObject>

class Task : public QObject
{
   Q_OBJECT
   Q_PROPERTY(int id READ getId WRITE setId NOTIFY idChanged)
   Q_PROPERTY(QString state READ getState WRITE setState NOTIFY stateChanged)
   Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
   Q_PROPERTY(QString result READ getResult WRITE setResult NOTIFY resultChanged)
   Q_PROPERTY(QString repo READ getRepo WRITE setRepo NOTIFY repoChanged)
   Q_PROPERTY(QString command READ getCommand WRITE setCommand NOTIFY commandChanged)
   Q_PROPERTY(int software READ getSoftware NOTIFY softwareChanged)
   Q_PROPERTY(int instance READ getInstance NOTIFY instanceChanged)
public:
   explicit Task(const QJsonObject& object);
   int getId();
   void setId(int id);
   QString getState();
   void setState(QString state);
   QString getName();
   void setName(QString name);
   QString getResult();
   void setResult(QString result);
   QString getRepo();
   void setRepo(QString repo);
   QString getCommand();
   void setCommand(QString command);
   int getSoftware();
   int getInstance();

signals:
   void idChanged();
   void stateChanged();
   void nameChanged();
   void resultChanged();
   void repoChanged();
   void commandChanged();
   void softwareChanged();
   void instanceChanged();

private:
   int mId;
   QString mState;
   QString mName;
   QString mResult;
   QString mRepo;
   QString mCommand;
   QString mSoftware;
   QString mInstance;
};

#endif // TASK_HPP
