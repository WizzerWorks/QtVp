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

#ifndef __VPGRIDDIALOG_H_
#define __VPGRIDDIALOG_H_

// Include Qt header files.
#include <QDialog>

// Include QtVp header files.
#include "vpgrid.h"


namespace Ui {
class VpGridDialog;
}

class VpGridDialog : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ getColor NOTIFY colorChanged)
    
  public:
    explicit VpGridDialog(QWidget *parent = 0);
    ~VpGridDialog();

    int getXSpacing();
    void setXSpacing(const int value);
    int getYSpacing();
    void setYSpacing(const int value);
    int getXAlignment();
    void setXAlignment(const int value);
    int getYAlignment();
    void setYAlignment(const int value);
    int getXResolution();
    void setXResolution(const int value);
    int getYResolution();
    void setYResolution(const int value);
    int getMultiplier();
    void setMultiplier(const int value);
    VpGrid::State getState();
    void setState(const VpGrid::State state);
    VpGrid::Style getStyle();
    void setStyle(const VpGrid::Style style);
    QColor getColor() { return m_color; }
    void setColor(const QColor color);
    VpGrid::RefState getReferenceState();
    void setReferenceState(const VpGrid::RefState state);
    VpGrid::RefStyle getReferenceStyle();
    void setReferenceStyle(const VpGrid::RefStyle style);
    QColor getReferenceColor() { return m_referenceColor; }
    void setReferenceColor(const QColor color);

  signals:
    void colorChanged(QColor color);
    void referenceColorChanged(QColor color);

  private slots:
    void on_xSpacingLineEdit_textChanged();
    void on_ySpacingLineEdit_textChanged();
    void on_xAlignmentLineEdit_textChanged();
    void on_yAlignmentLineEdit_textChanged();
    void on_xResolutionLineEdit_textChanged();
    void on_yResolutionLineEdit_textChanged();
    void on_multiplierLineEdit_textChanged();
    void on_stateComboBox_currentIndexChanged(int index);
    void on_styleComboBox_currentIndexChanged(int index);
    void on_colorWell_changed();
    void on_referenceStateComboBox_currentIndexChanged(int index);
    void on_referenceStyleComboBox_currentIndexChanged(int index);
    void on_referenceColorWell_changed();

  protected:
    bool eventFilter(QObject *obj, QEvent *ev);

  private:
    Ui::VpGridDialog *m_ui;
    QColor m_color;
    QColor m_referenceColor;

    bool validate();
};

#endif // __VPGRIDDIALOG_H_
