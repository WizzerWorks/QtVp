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
// COPYRIGHE_END

// Include Qt header files.
#include <QPushButton>
#include <QColorDialog>

// Inclde QtVp header files.
#include "vpgriddialog.h"
#include "ui_vpgriddialog.h"

VpGridDialog::VpGridDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::VpGridDialog)
{
    m_ui->setupUi(this);

    // Configure button box ("Ok" and "Cancel" buttons).
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(m_ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    // Configure line editor widgets.
    QRegExp regExp("[1-9][0-9]{0,20}");
    m_ui->xSpacingLineEdit->setValidator(new QRegExpValidator(regExp, this));
    m_ui->ySpacingLineEdit->setValidator(new QRegExpValidator(regExp, this));
    regExp = QRegExp("[0-9]");
    m_ui->xAlignmentLineEdit->setValidator(new QRegExpValidator(regExp, this));
    m_ui->yAlignmentLineEdit->setValidator(new QRegExpValidator(regExp, this));
    regExp = QRegExp("[1-9][0-9]{0,20}");
    m_ui->xResolutionLineEdit->setValidator(new QRegExpValidator(regExp, this));
    m_ui->yResolutionLineEdit->setValidator(new QRegExpValidator(regExp, this));
    regExp = QRegExp("[1-5]");
    m_ui->multiplierLineEdit->setValidator(new QRegExpValidator(regExp, this));

    // Configure color well, default color is black with no transparency.
    m_ui->colorWell->installEventFilter(this);
    m_color.setRed(0);
    m_color.setGreen(0);
    m_color.setBlue(0);
    m_color.setAlpha(255);
    on_colorWell_changed();

    // Configure reference color well, default color is black with no transparency.
    m_ui->referenceColorWell->installEventFilter(this);
    m_referenceColor.setRed(0);
    m_referenceColor.setGreen(0);
    m_referenceColor.setBlue(0);
    m_referenceColor.setAlpha(255);
    on_referenceColorWell_changed();
}

VpGridDialog::~VpGridDialog()
{
    delete m_ui;
}

int VpGridDialog::getXSpacing()
{
    QString text = m_ui->xSpacingLineEdit->text();
    int value = text.toInt();
    return value;
}

void VpGridDialog::setXSpacing(const int value)
{
    QString text;
    text = QString("%1").arg(value);
    m_ui->xSpacingLineEdit->setText(text);
}

int VpGridDialog::getYSpacing()
{
    QString text = m_ui->ySpacingLineEdit->text();
    int value = text.toInt();
    return value;
}

void VpGridDialog::setYSpacing(const int value)
{
    QString text;
    text = QString("%1").arg(value);
    m_ui->ySpacingLineEdit->setText(text);
}

int VpGridDialog::getXAlignment()
{
    QString text = m_ui->xAlignmentLineEdit->text();
    int value = text.toInt();
    return value;
}

void VpGridDialog::setXAlignment(const int value)
{
    QString text;
    text = QString("%1").arg(value);
    m_ui->xAlignmentLineEdit->setText(text);
}

int VpGridDialog::getYAlignment()
{
    QString text = m_ui->yAlignmentLineEdit->text();
    int value = text.toInt();
    return value;
}

void VpGridDialog::setYAlignment(const int value)
{
    QString text;
    text = QString("%1").arg(value);
    m_ui->yAlignmentLineEdit->setText(text);
}

int VpGridDialog::getXResolution()
{
    QString text = m_ui->xResolutionLineEdit->text();
    int value = text.toInt();
    return value;
}

void VpGridDialog::setXResolution(const int value)
{
    QString text;
    text = QString("%1").arg(value);
    m_ui->xResolutionLineEdit->setText(text);
}

int VpGridDialog::getYResolution()
{
    QString text = m_ui->yResolutionLineEdit->text();
    int value = text.toInt();
    return value;
}

void VpGridDialog::setYResolution(const int value)
{
    QString text;
    text = QString("%1").arg(value);
    m_ui->yResolutionLineEdit->setText(text);
}

int VpGridDialog::getMultiplier()
{
    QString text = m_ui->multiplierLineEdit->text();
    int value = text.toInt();
    return value;
}

void VpGridDialog::setMultiplier(const int value)
{
    QString text;
    text = QString("%1").arg(value);
    m_ui->multiplierLineEdit->setText(text);
}

VpGrid::State VpGridDialog::getState()
{
    QString text = m_ui->stateComboBox->currentText();
    if (text == QString("On"))
        return VpGrid::STATE_ON;
    else if (text == QString("Off"))
        return VpGrid::STATE_OFF;
    else if (text == QString("Hidden"))
        return VpGrid::STATE_HIDDEN;
    else
        return VpGrid::STATE_UNKNOWN;
}

void VpGridDialog::setState(const VpGrid::State state)
{
    if (state == VpGrid::STATE_ON)
        m_ui->stateComboBox->setCurrentText(QString("On"));
    else if (state == VpGrid::STATE_HIDDEN)
        m_ui->stateComboBox->setCurrentText(QString("Hidden"));
    else
        m_ui->stateComboBox->setCurrentText(QString("Off"));
}

VpGrid::Style VpGridDialog::getStyle()
{
    QString text = m_ui->styleComboBox->currentText();
    if (text == QString("Line"))
        return VpGrid::STYLE_LINE;
    else if (text == QString("Dot"))
        return VpGrid::STYLE_DOT;
    else if (text == QString("Cross"))
        return VpGrid::STYLE_CROSS;
    else
        return VpGrid::STYLE_UNKNOWN;
}

void VpGridDialog::setStyle(const VpGrid::Style style)
{
    if (style == VpGrid::STYLE_LINE)
        m_ui->styleComboBox->setCurrentText(QString("Line"));
    else if (style == VpGrid::STYLE_DOT)
        m_ui->styleComboBox->setCurrentText(QString("Dot"));
    else if (style == VpGrid::STYLE_CROSS)
        m_ui->styleComboBox->setCurrentText(QString("Cross"));
}

VpGrid::RefState VpGridDialog::getReferenceState()
{
    QString text = m_ui->referenceStateComboBox->currentText();
    if (text == QString("On"))
        return VpGrid::REFSTATE_ON;
    else if (text == QString("Off"))
        return VpGrid::REFSTATE_OFF;
    else
        return VpGrid::REFSTATE_UNKNOWN;
}

void VpGridDialog::setReferenceState(const VpGrid::RefState state)
{
    if (state == VpGrid::REFSTATE_ON)
        m_ui->referenceStateComboBox->setCurrentText(QString("On"));
    else
        m_ui->referenceStateComboBox->setCurrentText(QString("Off"));
}

VpGrid::RefStyle VpGridDialog::getReferenceStyle()
{
    QString text = m_ui->referenceStyleComboBox->currentText();
    if (text == QString("Square"))
        return VpGrid::REFSTYLE_SQUARE;
    else if (text == QString("Circle"))
        return VpGrid::REFSTYLE_CIRCLE;
    else if (text == QString("Cross"))
        return VpGrid::REFSTYLE_CROSS;
    else
        return VpGrid::REFSTYLE_UNKNOWN;
}

void VpGridDialog::setReferenceStyle(const VpGrid::RefStyle style)
{
    if (style == VpGrid::REFSTYLE_SQUARE)
        m_ui->referenceStyleComboBox->setCurrentText(QString("Square"));
    else if (style == VpGrid::REFSTYLE_CIRCLE)
        m_ui->referenceStyleComboBox->setCurrentText(QString("Circle"));
    else if (style == VpGrid::REFSTYLE_CROSS)
        m_ui->referenceStyleComboBox->setCurrentText(QString("Cross"));
}

void VpGridDialog::setColor(const QColor color)
{
    m_color = color;
    on_colorWell_changed();
}

void VpGridDialog::setReferenceColor(const QColor color)
{
    m_referenceColor = color;
    on_referenceColorWell_changed();
}

void VpGridDialog::on_xSpacingLineEdit_textChanged()
{
    //qDebug("xSpacing text changed.");
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
        validate());
}

void VpGridDialog::on_ySpacingLineEdit_textChanged()
{
    //qDebug("ySpacing text changed.");
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
        validate());
}

void VpGridDialog::on_xAlignmentLineEdit_textChanged()
{
    //qDebug("xAlignment text changed.");
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
        validate());
}

void VpGridDialog::on_yAlignmentLineEdit_textChanged()
{
    //qDebug("yAlignment text changed.");
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
        validate());
}

void VpGridDialog::on_xResolutionLineEdit_textChanged()
{
    //qDebug("xResolution text changed.");
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
        validate());
}

void VpGridDialog::on_yResolutionLineEdit_textChanged()
{
    //qDebug("yResolution text changed.");
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
        validate());
}

void VpGridDialog::on_multiplierLineEdit_textChanged()
{
    //qDebug("xResolution text changed.");
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
        validate());
}

void VpGridDialog::on_stateComboBox_currentIndexChanged(int index)
{
    QString msg;
    msg = QString("state changed to %1.").arg(index);
    //qDebug(msg.toUtf8());
}


void VpGridDialog::on_styleComboBox_currentIndexChanged(int index)
{
    QString msg;
    msg = QString("style changed to %1.").arg(index);
    //qDebug(msg.toUtf8());
}


void VpGridDialog::on_referenceStateComboBox_currentIndexChanged(int index)
{
    QString msg;
    msg = QString("reference changed to %1.").arg(index);
    //qDebug(msg.toUtf8());
}

void VpGridDialog::on_referenceStyleComboBox_currentIndexChanged(int index)
{
    QString msg;
    msg = QString("reference changed to %1.").arg(index);
    //qDebug(msg.toUtf8());
}

 void VpGridDialog::on_colorWell_changed()
 {
     QPalette pal = m_ui->colorWell->palette();
     pal.setColor(QPalette::Window, m_color);
     m_ui->colorWell->setPalette(pal);
     emit colorChanged(m_color);
 }

 void VpGridDialog::on_referenceColorWell_changed()
 {
     QPalette pal = m_ui->referenceColorWell->palette();
     pal.setColor(QPalette::Window, m_referenceColor);
     m_ui->referenceColorWell->setPalette(pal);
     emit referenceColorChanged(m_referenceColor);
 }

 bool VpGridDialog::eventFilter(QObject *obj, QEvent *ev)
 {
     if ((obj == m_ui->colorWell) || (obj == m_ui->referenceColorWell))
     {
         if (ev->type() == QEvent::MouseButtonRelease)
         {
             //QMouseEvent *event = static_cast<QMouseEvent*>(ev);
             QColorDialog *colorDialog = new QColorDialog();
             if (obj == m_ui->colorWell)
                 // Modify grid color.
                 colorDialog->setCurrentColor(getColor());
             else
                 // Modify reference color.
                 colorDialog->setCurrentColor(getReferenceColor());
             int retValue = colorDialog->exec();
             if (retValue == QDialog::Accepted)
             {
                 QColor selectedColor = colorDialog->currentColor();
                 if (obj == m_ui->colorWell)
                     // Modify grid color.
                     setColor(selectedColor);
                 else
                     // Modify reference color.
                     setReferenceColor(selectedColor);
             }
             delete colorDialog;
             return true;
         } else {
             return false;
         }
     } else {
         // Pass the event on to the parent class.
         return QDialog::eventFilter(obj, ev);
     }
 }

bool VpGridDialog::validate()
{
    if (m_ui->xSpacingLineEdit->hasAcceptableInput() &&
        m_ui->ySpacingLineEdit->hasAcceptableInput() &&
        m_ui->xAlignmentLineEdit->hasAcceptableInput() &&
        m_ui->yAlignmentLineEdit->hasAcceptableInput() &&
        m_ui->xResolutionLineEdit->hasAcceptableInput() &&
        m_ui->yResolutionLineEdit->hasAcceptableInput() &&
        m_ui->multiplierLineEdit->hasAcceptableInput())
        return true;
    else
        return false;
}
