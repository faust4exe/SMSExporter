#include <QApplication>
#include <QtDeclarative>

#include "qmlapplicationviewer.h"
#include "SMSExporter.h"

QTM_USE_NAMESPACE

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QmlApplicationViewer viewer;

    qmlRegisterType<SMSExporter>("MyComponents", 1, 1, "SMSExporter");

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/SMSExporter/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
