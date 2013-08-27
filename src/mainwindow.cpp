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

// Include QtVp library header files.
#include "vputil.h"
#include "mainwindow.h"
#include "vpgriddialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_view(NULL)
{
    m_ui->setupUi(this);

    m_view = new VpGraphicsView();

    // Set up default viewport and grid.
    m_vp = new VpGraphics2D();
    m_vp->getGrid()->setState(VpGrid::STATE_ON);
    m_vp->getGrid()->setStyle(VpGrid::STYLE_LINE);
    m_vp->getGrid()->setXSpacing(10);
    m_vp->getGrid()->setYSpacing(10);
    m_vp->getGrid()->setColor(VpColor(QColor(145, 145 ,145, 255)));
    m_vp->getGrid()->setReferenceState(VpGrid::REFSTATE_ON);
    QString cmd = m_vp->toString();

    // Listen for changes to coord values.
    connect(m_vp, SIGNAL(coordChanged(QString)), this, SLOT(on_updateStatus(QString)));
    connect(m_vp, SIGNAL(updateStatus(QString)), this, SLOT(on_updateStatus(QString)));

    m_view->init(m_vp);

    //centralWidget()->resize(QSize(100, 100));
    //m_vp->setMinimumSize(QSize(100, 100));
    //m_vp->resize(100, 100);
    //setCentralWidget(m_vp);
    setCentralWidget(m_view);
}

MainWindow::~MainWindow()
{
    if (m_view != NULL)
        delete m_view;
    delete m_ui;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Viewport Test"),
            tr("<h2>Viewport Test 0.1</h2>"
               "<p>Copyright &copy; 2013 Wizzer Works"
               "<p>Viewport Test is used to exercise "
               "the Wizzer Works QtVp library."));
}

void MainWindow::zoomIn()
{
    int x_min = m_vp->getWxmin();
    int x_max = m_vp->getWxmax();
    int y_min = m_vp->getWymin();
    int y_max = m_vp->getWymax();

    // Zoom in by 50%.
    float halfx = (x_max - x_min) / 2;
    float halfy = (y_max - y_min) / 2;
    x_min = VpUtil::round(x_min + halfx/2);
    x_max = VpUtil::round(x_max - halfx/2);
    y_min = VpUtil::round(y_min + halfy/2);
    y_max = VpUtil::round(y_max - halfy/2);

    m_vp->setWorldCoords(x_min, y_min, x_max, y_max);

    // Refresh the display.
    m_vp->update();
}

void MainWindow::zoomOut()
{
    int x_min = m_vp->getWxmin();
    int x_max = m_vp->getWxmax();
    int y_min = m_vp->getWymin();
    int y_max = m_vp->getWymax();

    // Zoom out by 50%.
    float halfx = (x_max - x_min) / 2;
    float halfy = (y_max - y_min) / 2;
    x_min = VpUtil::round(x_min - halfx/2);
    x_max = VpUtil::round(x_max + halfx/2);
    y_min = VpUtil::round(y_min - halfy/2);
    y_max = VpUtil::round(y_max + halfy/2);

    m_vp->setWorldCoords(x_min, y_min, x_max, y_max);

    // Refresh the display.
    m_vp->update();
}

void MainWindow::center()
{
    int x_min = m_vp->getWxmin();
    int x_max = m_vp->getWxmax();
    int y_min = m_vp->getWymin();
    int y_max = m_vp->getWymax();

    // Center the world coordinate system, placing (0,0)
    // in the middle of the widget.
    float halfx = (x_max - x_min) / 2;
    x_min = VpUtil::round(-halfx);
    x_max = VpUtil::round(halfx);
    float halfy = (y_max - y_min) / 2;
    y_min = VpUtil::round(-halfy);
    y_max = VpUtil::round(halfy);

    m_vp->setWorldCoords(x_min, y_min, x_max, y_max);

    // Refresh the display.
    m_vp->update();
}

void MainWindow::on_actionAbout_triggered()
{
    about();
}

void MainWindow::on_actionZoom_In_triggered()
{
    //qDebug("Selected Zoom In menu.");
    zoomIn();
}

void MainWindow::on_actionZoom_Out_triggered()
{
    //qDebug("Selected Zoom Out menu.");
    zoomOut();
}

void MainWindow::on_actionCenter_triggered()
{
    //qDebug("Selected Center menu.");
    center();
}

void MainWindow::on_actionGrid_triggered()
{
    //qDebug("Selected Grid menu.");

    // Create a new dialog for changing the grid display properties.
    VpGridDialog *gridDialog = new VpGridDialog();

    // Populate the dialog with current display property values.
    gridDialog->setXSpacing(m_vp->getGrid()->getXSpacing());
    gridDialog->setYSpacing(m_vp->getGrid()->getYSpacing());
    gridDialog->setXAlignment(m_vp->getGrid()->getXAlignment());
    gridDialog->setYAlignment(m_vp->getGrid()->getYAlignment());
    gridDialog->setXResolution(m_vp->getGrid()->getXResolution());
    gridDialog->setYResolution(m_vp->getGrid()->getYResolution());
    gridDialog->setMultiplier(m_vp->getGrid()->getMultiplier());
    gridDialog->setState(m_vp->getGrid()->getState());
    gridDialog->setStyle(m_vp->getGrid()->getStyle());
    gridDialog->setColor(m_vp->getGrid()->getColor());
    gridDialog->setReferenceState(m_vp->getGrid()->getReferenceState());
    gridDialog->setReferenceStyle(m_vp->getGrid()->getReferenceStyle());
    gridDialog->setReferenceColor(m_vp->getGrid()->getReferenceColor());

    // Show the display and wait for user input.
    int retValue = gridDialog->exec();
    if (retValue == QDialog::Accepted)
    {
         // Update the grid with the modified property values.
        int xSpacing = gridDialog->getXSpacing();
        int ySpacing = gridDialog->getYSpacing();
        int xAlignment = gridDialog->getXAlignment();
        int yAlignment = gridDialog->getYAlignment();
        int xResolution = gridDialog->getXResolution();
        int yResolution = gridDialog->getYResolution();
        int multiplier = gridDialog->getMultiplier();
        VpGrid::State state = gridDialog->getState();
        VpGrid::Style style = gridDialog->getStyle();
        QColor color = gridDialog->getColor();
        VpGrid::RefState refState = gridDialog->getReferenceState();
        VpGrid::RefStyle refStyle = gridDialog->getReferenceStyle();
        QColor refColor = gridDialog->getReferenceColor();

        m_vp->getGrid()->setXSpacing(xSpacing);
        m_vp->getGrid()->setYSpacing(ySpacing);
        m_vp->getGrid()->setXAlignment(xAlignment);
        m_vp->getGrid()->setYAlignment(yAlignment);
        m_vp->getGrid()->setXResolution(xResolution);
        m_vp->getGrid()->setYResolution(yResolution);
        m_vp->getGrid()->setMultiplier(multiplier);
        m_vp->getGrid()->setState(state);
        m_vp->getGrid()->setStyle(style);
        m_vp->getGrid()->setColor(VpColor(color));
        m_vp->getGrid()->setReferenceState(refState);
        m_vp->getGrid()->setReferenceStyle(refStyle);
        m_vp->getGrid()->setReferenceColor(refColor);

        // Signal the viewport to repaint itself.
        m_vp->update();
    }

    // Clean up.
    delete gridDialog;
}

void MainWindow::on_updateStatus(const QString coord)
{
    //qDebug(coord.toLocal8Bit().data());
    statusBar()->clearMessage();
    statusBar()->showMessage(coord);
}
