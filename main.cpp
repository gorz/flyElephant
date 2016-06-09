#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Controller.hpp"

int main(int argc, char *argv[])
{
   QGuiApplication app(argc, argv);

   Controller controller;

   QQmlApplicationEngine engine;
   QQmlContext* context = engine.rootContext();
   context->setContextProperty("controller", &controller);
   context->setContextProperty("tasks", controller.getTasks());
   engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

   return app.exec();
}

