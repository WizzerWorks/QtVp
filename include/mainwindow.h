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

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

// Include Qt header files.
#include <QMainWindow>

// Include QtVp header files.
#include "vpgraphics2D.h"
#include "vpgraphicsview.h"
#include "vpgraphics2d.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
  public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  protected:

    void about();
    void zoomIn();
    void zoomOut();
    void center();
    
  private slots:

    void on_actionAbout_triggered();

    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionCenter_triggered();

    void on_actionGrid_triggered();

    void on_updateStatus(const QString coord);

  private:

    Ui::MainWindow *m_ui;
    VpGraphicsView *m_view;
    VpGraphics2D *m_vp;
};

#endif // __MAINWINDOW_H_
