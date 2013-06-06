/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the Qt Commercial Charts Add-on.
**
** $QT_BEGIN_LICENSE$
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qboxset.h"
#include "qboxset_p.h"
#include "charthelpers_p.h"

QTCOMMERCIALCHART_BEGIN_NAMESPACE

/*!
    \class QBoxSet
    \brief Building block for box-and-whiskers chart

    QBoxSet represents one box-and-whiskers item. It takes five values to create a graphical representation
    of range and three medians. There are two ways to give the values. The first one is with constructor
    or stream operator (<<). The values have to be given in the following order: lower extreme,
    lower quartile, median, upper quartile and upper extreme. The Second method is to create an empty QBoxSet instance and
    give the values using setValue method.

    \mainclass

    \sa QBoxPlotSeries
*/
/*!
    \qmlclass BoxSet QBoxSet

    BoxSet represents one box-and-whiskers item. It takes five values to create a graphical representation
    of range and three medians. There are two ways to give the values. The first one is with constructor
    or with append method. In these the values have to be given in the following order: lower extreme, lower quartile, median,
    upper quartile and upper extre. The second method is to create an empty QBoxSet instance and give the values using
    value specific methods.
    \sa BoxPlotSeries
*/
/*!
    \enum QBoxSet::ValuePositions

    \value LowerExtreme
    \value LowerQuartile
    \value Median
    \value UpperQuartile
    \value UpperExtreme
*/

/*!
    \qmlproperty string BoxSet::label
    Defines the label of the boxSet.
*/
/*!
    \qmlproperty int BoxSet::count
    The count of values on the box-and-whiskers set
*/

/*!
    \property QBoxSet::pen
    \brief Defines the pen used by the box-and-whiskers set.
*/

/*!
    \property QBoxSet::brush
    \brief Defines the brush used by the box-and-whiskers set.
*/

/*!
    \qmlmethod void BoxSet::setValue(int index, qreal value)
    Sets a new \a value on the \a index position.
*/
/*!
    \fn void QBoxSet::clicked()
    The signal is emitted if the user clicks with a mouse on top of box-and-whisker item.
*/
/*!
    \qmlsignal BoxSet::onClicked()
    This signal is emitted when the user clicks with a mouse on top of box-and-whisker item.
*/

/*!
    \fn void QBoxSet::hovered(bool status)

    The signal is emitted if mouse is hovered on top of box-and-whisker item.
    Parameter \a status is true, if mouse entered on top of item, false if mouse left from top of item.
*/
/*!
    \qmlsignal BoxSet::onHovered(bool status)

    The signal is emitted if mouse is hovered on top of box-and-whisker item.
    Parameter \a status is true, if mouse entered on top of item, false if mouse left from top of item.
*/

/*!
    \fn void QBoxSet::penChanged()
    This signal is emitted when the pen of the box-and-whisker item has changed.
    \sa pen
*/
/*!
    \qmlsignal BoxSet::onPenChanged()
    This signal is emitted when the pen of the box-and-whisker item has changed.
*/
/*!
    \fn void QBoxSet::brushChanged()
    This signal is emitted when the brush of the box-and-whisker item has changed.
    \sa brush
*/
/*!
    \qmlsignal BoxSet::onBrushChanged()
    This signal is emitted when the brush of the box-and-whisker item has changed.
*/

/*!
    \fn void QBoxSet::valuesChanged()
    This signal is emitted when multiple values have been changed on the box-and-whisker item.
    \sa append()
*/
/*!
    \qmlsignal BoxSet::onChangedValues()
    This signal is emitted when multiple values have been changed on the box-and-whisker item.
*/

/*!
    \fn void QBoxSet::valueChanged(int index)
    This signal is emitted values the value in the box-and-whisker item has been modified.
    Parameter \a index indicates the position of the modified value.
    \sa at()
*/
/*!
    \qmlsignal BoxSet::onChangedValue(int index)
    This signal is emitted values the value in the box-and-whisker item has been modified.
    Parameter \a index indicates the position of the modified value.
*/

/*!
    \fn void QBoxSet::cleared()
    This signal is emitted when all the values on the set are cleared to 0.
*/
/*!
    \qmlsignal BoxSet::onCleared()
    This signal is emitted when all the values on the set are cleared to 0.
*/


/*!
    Constructs QBoxSet with optional \a label and parent of \a parent
*/
QBoxSet::QBoxSet(const QString label, QObject *parent)
    : QObject(parent),
      d_ptr(new QBoxSetPrivate(label, this))
{
}

/*!
    Constructs QBoxSet with given ordered values. \a le for lower extreme, \a lq for lower quartile, \a m for median,
    \a uq for upper quartile and \a ue for upper quartile. \a label and \a parent are optional.
 */
QBoxSet::QBoxSet(const qreal le, const qreal lq, const qreal m, const qreal uq, const qreal ue, const QString label, QObject *parent)
    : QObject(parent),
      d_ptr(new QBoxSetPrivate(label, this))
{
    d_ptr->append(le);
    d_ptr->append(lq);
    d_ptr->append(m);
    d_ptr->append(uq);
    d_ptr->append(ue);
}

/*!
    Destroys the boxset
*/
QBoxSet::~QBoxSet()
{
}

/*!
    Appends new value \a value to the end of set.
*/
void QBoxSet::append(const qreal value)
{
    if (d_ptr->append(value))
        emit valueChanged(d_ptr->m_appendCount - 1);
}

/*!
    Appends a list of reals to set. Works like append with single real value. The \a values in list
    are appended to end of boxset.
    \sa append()
*/
void QBoxSet::append(const QList<qreal> &values)
{
    if (d_ptr->append(values))
        emit valuesChanged();
}

/*!
    Sets new \a label for set.
*/
void QBoxSet::setLabel(const QString label)
{
    d_ptr->m_label = label;
}

/*!
    Returns label of the set.
*/
QString QBoxSet::label() const
{
    return d_ptr->m_label;
}

/*!
    Convenience operator. Same as append, with real \a value.
    \sa append()
*/
QBoxSet &QBoxSet::operator << (const qreal &value)
{
    append(value);
    return *this;
}

/*!
    Sets a new \a value on the \a index position. For \a index ValuePositions can be used.
*/
void QBoxSet::setValue(const int index, const qreal value)
{
    d_ptr->setValue(index, value);
    emit valueChanged(index);
}

/*!
    Sets all values on the set to 0.
 */
void QBoxSet::clear()
{
    d_ptr->clear();
    emit cleared();
}

/*!
    Returns value of set indexed by \a index. For \a index ValuePositions can be used.
    If the index is out of bounds 0.0 is returned.
*/
qreal QBoxSet::at(const int index) const
{
    if (index < 0 || index >= 5)
        return 0;
    return d_ptr->m_values[index];
}

/*!
    Returns value of set indexed by \a index. For \a index ValuePositions can be used.
    If the index is out of bounds 0.0 is returned.
*/
qreal QBoxSet::operator [](const int index) const
{
    return at(index);
}

/*!
    Returns count of values appended to the set.
*/
int QBoxSet::count() const
{
    return d_ptr->m_appendCount;
}

/*!
    Sets pen for set. Boxes of this set are drawn using \a pen
*/
void QBoxSet::setPen(const QPen &pen)
{
    if (d_ptr->m_pen != pen) {
        d_ptr->m_pen = pen;
        emit d_ptr->updatedBox();
        emit penChanged();
    }
}

/*!
    Returns pen of the set.
*/
QPen QBoxSet::pen() const
{
    return d_ptr->m_pen;
}

/*!
    Sets brush for the set. Boxes of this set are drawn using \a brush
*/
void QBoxSet::setBrush(const QBrush &brush)
{
    if (d_ptr->m_brush != brush) {
        d_ptr->m_brush = brush;
        emit d_ptr->updatedBox();
        emit brushChanged();
    }
}

/*!
    Returns brush of the set.
*/
QBrush QBoxSet::brush() const
{
    return d_ptr->m_brush;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QBoxSetPrivate::QBoxSetPrivate(const QString label, QBoxSet *parent) : QObject(parent),
    q_ptr(parent),
    m_label(label),
    m_valuesCount(5),
    m_appendCount(0),
    m_pen(QPen(Qt::NoPen)),
    m_brush(QBrush(Qt::NoBrush))
{
    m_values = new qreal[m_valuesCount];
}

QBoxSetPrivate::~QBoxSetPrivate()
{
}

bool QBoxSetPrivate::append(qreal value)
{
    if (isValidValue(value) && m_appendCount < m_valuesCount) {
        m_values[m_appendCount++] = value;
        emit restructuredBox();

        return true;
    }
    return false;
}

bool QBoxSetPrivate::append(QList<qreal> values)
{
    bool success = false;

    for (int i = 0; i < values.count(); i++) {
        if (isValidValue(values.at(i)) && m_appendCount < m_valuesCount) {
            success = true;
            m_values[m_appendCount++] = values.at(i);
        }
    }

    if (success)
        emit restructuredBox();

    return success;
}

void QBoxSetPrivate::clear()
{
    m_appendCount = 0;
    for (int i = 0; i < m_valuesCount; i++)
         m_values[i] = 0.0;
    emit restructuredBox();
}

void QBoxSetPrivate::setValue(const int index, const qreal value)
{
    if (index < m_valuesCount) {
        m_values[index] = value;
        emit updatedLayout();
    }
}

qreal QBoxSetPrivate::value(const int index)
{
    if (index < 0 || index >= m_valuesCount)
        return 0;
    return m_values[index];
}

#include "moc_qboxset.cpp"
#include "moc_qboxset_p.cpp"

QTCOMMERCIALCHART_END_NAMESPACE