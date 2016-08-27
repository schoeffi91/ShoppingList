#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include "src/shoppinglistmodel.h"
#include "src/smtpclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    ShoppingListModel shoppingModel;
    SmtpClient smtpClient;

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.rootContext()->setContextProperty("ShoppingModel", &shoppingModel);
    view.rootContext()->setContextProperty("SmtpClient", &smtpClient);
    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.show();

    return app.exec();
}
