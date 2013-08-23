// COPYRIGHT_BEGIN
// The MIT License (MIT)
//
// Copyright (c) 2013 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// COPYRIGHT_END

// Include Qt header files.
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>

// Include QtVp library header files.
#include "mainwindow.h"

// The global Qt Application.
QApplication *g_app;

// Utility class for displaying splash.
class SplashUtil : public QThread
{
  public:
    static void sleep(unsigned long secs) {
        QThread::sleep(secs);
    }
};

int main(int argc, char *argv[])
{
    g_app = new QApplication(argc, argv);

    // Create a splash screen.
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/images/splash.bmp"));
    splash->show();

    // Set message alignment for splash screen.
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    // Display message while bringing up main window state.
    splash->showMessage(QObject::tr("Setting up the main window ..."),
        topRight, Qt::black);
    g_app->processEvents(); // Accept a click on the screen so that user can cancel the screen.

    // Switch from splash state to main window.
    MainWindow mainWindow;
    SplashUtil::sleep(3); // Splash page is shown for 3 seconds.

    mainWindow.show();
    splash->finish(&mainWindow);
    delete splash;

    return g_app->exec();
}
