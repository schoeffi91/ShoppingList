#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>

#include "src/shoppinglistmodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    ShoppingListModel shoppingModel;

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.rootContext()->setContextProperty("ShoppingModel", &shoppingModel);
    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.show();

    return app.exec();
}
