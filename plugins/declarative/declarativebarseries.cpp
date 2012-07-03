/****************************************************************************
**
** Copyright (C) 2012 Digia Plc
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

#include "declarativebarseries.h"
#include "declarativechart.h"
#include <QBarSet>
#include <QVBarModelMapper>
#include <QHBarModelMapper>
#include <QAbstractAxis>

QTCOMMERCIALCHART_BEGIN_NAMESPACE

DeclarativeBarSet::DeclarativeBarSet(QObject *parent) :
    QBarSet("", parent)
{
    connect(this, SIGNAL(valuesAdded(int,int)), this, SLOT(handleCountChanged(int, int)));
    connect(this, SIGNAL(valuesRemoved(int,int)), this, SLOT(handleCountChanged(int, int)));
}

void DeclarativeBarSet::handleCountChanged(int index, int count)
{
    Q_UNUSED(index)
    Q_UNUSED(count)
    emit countChanged(QBarSet::count());
}

QVariantList DeclarativeBarSet::values()
{
    QVariantList values;
    for (int i(0); i < count(); i++)
        values.append(QVariant(QBarSet::at(i)));
    return values;
}

void DeclarativeBarSet::setValues(QVariantList values)
{
    while (count())
        remove(count() - 1);

    for (int i(0); i < values.count(); i++) {
        if (values.at(i).canConvert(QVariant::Double))
            QBarSet::append(values[i].toDouble());
    }
}

DeclarativeAbstractBarSeries::DeclarativeAbstractBarSeries(QDeclarativeItem *parent) :
    QAbstractBarSeries(parent)
{
    connect(this, SIGNAL(barsetsAdded(QList<QBarSet*>)), this, SLOT(handleAdded(QList<QBarSet*>)));
    connect(this, SIGNAL(barsetsRemoved(QList<QBarSet*>)), this, SLOT(handleRemoved(QList<QBarSet*>)));
}

void DeclarativeAbstractBarSeries::handleAdded(QList<QBarSet* > barsets)
{
    foreach(QBarSet *b, barsets) {
        DeclarativeBarSet *barset = qobject_cast<DeclarativeBarSet *>(b);
        emit added(barset);
    }
}

void DeclarativeAbstractBarSeries::handleRemoved(QList<QBarSet* > barsets)
{
    foreach(QBarSet *b, barsets) {
        DeclarativeBarSet *barset = qobject_cast<DeclarativeBarSet *>(b);
        emit removed(barset);
    }
}

void DeclarativeAbstractBarSeries::classBegin()
{
}

void DeclarativeAbstractBarSeries::componentComplete()
{
    foreach(QObject *child, children()) {
        if (qobject_cast<DeclarativeBarSet *>(child)) {
            QAbstractBarSeries::append(qobject_cast<DeclarativeBarSet *>(child));
        } else if (qobject_cast<QVBarModelMapper *>(child)) {
            QVBarModelMapper *mapper = qobject_cast<QVBarModelMapper *>(child);
            mapper->setSeries(this);
        } else if (qobject_cast<QHBarModelMapper *>(child)) {
            QHBarModelMapper *mapper = qobject_cast<QHBarModelMapper *>(child);
            mapper->setSeries(this);
        }
    }
}

void DeclarativeAbstractBarSeries::setAxisX(QAbstractAxis *axis)
{
    chart()->setAxisX(axis, this);
}

QAbstractAxis *DeclarativeAbstractBarSeries::axisX()
{
    return chart()->axisX(this);
}

void DeclarativeAbstractBarSeries::setAxisY(QAbstractAxis *axis)
{
    chart()->setAxisY(axis, this);
}

QAbstractAxis *DeclarativeAbstractBarSeries::axisY()
{
    return chart()->axisY(this);
}

QDeclarativeListProperty<QObject> DeclarativeAbstractBarSeries::seriesChildren()
{
    return QDeclarativeListProperty<QObject>(this, 0, &DeclarativeAbstractBarSeries::appendSeriesChildren);
}

void DeclarativeAbstractBarSeries::appendSeriesChildren(QDeclarativeListProperty<QObject> * list, QObject *element)
{
    // Empty implementation; the children are parsed in componentComplete instead
    Q_UNUSED(list);
    Q_UNUSED(element);
}

DeclarativeBarSet *DeclarativeAbstractBarSeries::at(int index)
{
    QList<QBarSet*> setList = barSets();
    if (index >= 0 && index < setList.count())
        return qobject_cast<DeclarativeBarSet *>(setList[index]);

    return 0;
}

DeclarativeBarSet *DeclarativeAbstractBarSeries::insert(int index, QString label, QVariantList values)
{
    int insertIndex = index;
    if (insertIndex < 0)
        insertIndex = 0;
    else if (insertIndex > count())
        insertIndex = count();

    DeclarativeBarSet *barset = new DeclarativeBarSet(this);
    barset->setLabel(label);
    barset->setValues(values);
    if (QAbstractBarSeries::insert(insertIndex, barset))
        return barset;
    delete barset;
    return 0;
}

DeclarativeBarSeries::DeclarativeBarSeries(QDeclarativeItem *parent) :
    QBarSeries(parent)
{
}

void DeclarativeBarSeries::classBegin()
{
}

void DeclarativeBarSeries::componentComplete()
{
    foreach(QObject *child, children()) {
        if (qobject_cast<DeclarativeBarSet *>(child)) {
            QAbstractBarSeries::append(qobject_cast<DeclarativeBarSet *>(child));
        } else if(qobject_cast<QVBarModelMapper *>(child)) {
            QVBarModelMapper *mapper = qobject_cast<QVBarModelMapper *>(child);
            mapper->setSeries(this);
        } else if(qobject_cast<QHBarModelMapper *>(child)) {
            QHBarModelMapper *mapper = qobject_cast<QHBarModelMapper *>(child);
            mapper->setSeries(this);
        }
    }
}

void DeclarativeBarSeries::setAxisX(QAbstractAxis *axis)
{
    chart()->setAxisX(axis, this);
}

QAbstractAxis *DeclarativeBarSeries::axisX()
{
    return chart()->axisX(this);
}

void DeclarativeBarSeries::setAxisY(QAbstractAxis *axis)
{
    chart()->setAxisY(axis, this);
}

QAbstractAxis *DeclarativeBarSeries::axisY()
{
    return chart()->axisY(this);
}

QDeclarativeListProperty<QObject> DeclarativeBarSeries::seriesChildren()
{
    return QDeclarativeListProperty<QObject>(this, 0, &DeclarativeAbstractBarSeries::appendSeriesChildren);
}

void DeclarativeBarSeries::appendSeriesChildren(QDeclarativeListProperty<QObject> * list, QObject *element)
{
    // Empty implementation; the children are parsed in componentComplete instead
    Q_UNUSED(list);
    Q_UNUSED(element);
}

DeclarativeBarSet *DeclarativeBarSeries::at(int index)
{
    QList<QBarSet*> setList = barSets();
    if (index >= 0 && index < setList.count())
        return qobject_cast<DeclarativeBarSet *>(setList[index]);

    return 0;
}

DeclarativeBarSet *DeclarativeBarSeries::insert(int index, QString label, QVariantList values)
{
    DeclarativeBarSet *barset = new DeclarativeBarSet(this);
    barset->setLabel(label);
    barset->setValues(values);
    if (QBarSeries::insert(index, barset))
        return barset;
    delete barset;
    return 0;
}

DeclarativeStackedBarSeries::DeclarativeStackedBarSeries(QDeclarativeItem *parent) :
    QStackedBarSeries(parent)
{
}

void DeclarativeStackedBarSeries::classBegin()
{
}

void DeclarativeStackedBarSeries::componentComplete()
{
    foreach(QObject *child, children()) {
        if (qobject_cast<DeclarativeBarSet *>(child)) {
            QAbstractBarSeries::append(qobject_cast<DeclarativeBarSet *>(child));
        } else if(qobject_cast<QVBarModelMapper *>(child)) {
            QVBarModelMapper *mapper = qobject_cast<QVBarModelMapper *>(child);
            mapper->setSeries(this);
        } else if(qobject_cast<QHBarModelMapper *>(child)) {
            QHBarModelMapper *mapper = qobject_cast<QHBarModelMapper *>(child);
            mapper->setSeries(this);
        }
    }
}

void DeclarativeStackedBarSeries::setAxisX(QAbstractAxis *axis)
{
    chart()->setAxisX(axis, this);
}

QAbstractAxis *DeclarativeStackedBarSeries::axisX()
{
    return chart()->axisX(this);
}

void DeclarativeStackedBarSeries::setAxisY(QAbstractAxis *axis)
{
    chart()->setAxisY(axis, this);
}

QAbstractAxis *DeclarativeStackedBarSeries::axisY()
{
    return chart()->axisY(this);
}

QDeclarativeListProperty<QObject> DeclarativeStackedBarSeries::seriesChildren()
{
    return QDeclarativeListProperty<QObject>(this, 0, &DeclarativeAbstractBarSeries::appendSeriesChildren);
}

void DeclarativeStackedBarSeries::appendSeriesChildren(QDeclarativeListProperty<QObject> * list, QObject *element)
{
    // Empty implementation; the children are parsed in componentComplete instead
    Q_UNUSED(list);
    Q_UNUSED(element);
}

DeclarativeBarSet *DeclarativeStackedBarSeries::at(int index)
{
    QList<QBarSet*> setList = barSets();
    if (index >= 0 && index < setList.count())
        return qobject_cast<DeclarativeBarSet *>(setList[index]);

    return 0;
}

DeclarativeBarSet *DeclarativeStackedBarSeries::insert(int index, QString label, QVariantList values)
{
    DeclarativeBarSet *barset = new DeclarativeBarSet(this);
    barset->setLabel(label);
    barset->setValues(values);
    if (QStackedBarSeries::insert(index, barset))
        return barset;
    delete barset;
    return 0;
}

DeclarativePercentBarSeries::DeclarativePercentBarSeries(QDeclarativeItem *parent) :
    QPercentBarSeries(parent)
{
}

void DeclarativePercentBarSeries::classBegin()
{
}

void DeclarativePercentBarSeries::componentComplete()
{
    foreach(QObject *child, children()) {
        if (qobject_cast<DeclarativeBarSet *>(child)) {
            QAbstractBarSeries::append(qobject_cast<DeclarativeBarSet *>(child));
        } else if(qobject_cast<QVBarModelMapper *>(child)) {
            QVBarModelMapper *mapper = qobject_cast<QVBarModelMapper *>(child);
            mapper->setSeries(this);
        } else if(qobject_cast<QHBarModelMapper *>(child)) {
            QHBarModelMapper *mapper = qobject_cast<QHBarModelMapper *>(child);
            mapper->setSeries(this);
        }
    }
}

void DeclarativePercentBarSeries::setAxisX(QAbstractAxis *axis)
{
    chart()->setAxisX(axis, this);
}

QAbstractAxis *DeclarativePercentBarSeries::axisX()
{
    return chart()->axisX(this);
}

void DeclarativePercentBarSeries::setAxisY(QAbstractAxis *axis)
{
    chart()->setAxisY(axis, this);
}

QAbstractAxis *DeclarativePercentBarSeries::axisY()
{
    return chart()->axisY(this);
}

QDeclarativeListProperty<QObject> DeclarativePercentBarSeries::seriesChildren()
{
    return QDeclarativeListProperty<QObject>(this, 0, &DeclarativeAbstractBarSeries::appendSeriesChildren);
}

void DeclarativePercentBarSeries::appendSeriesChildren(QDeclarativeListProperty<QObject> * list, QObject *element)
{
    // Empty implementation; the children are parsed in componentComplete instead
    Q_UNUSED(list);
    Q_UNUSED(element);
}

DeclarativeBarSet *DeclarativePercentBarSeries::at(int index)
{
    QList<QBarSet*> setList = barSets();
    if (index >= 0 && index < setList.count())
        return qobject_cast<DeclarativeBarSet *>(setList[index]);

    return 0;
}

DeclarativeBarSet *DeclarativePercentBarSeries::insert(int index, QString label, QVariantList values)
{
    DeclarativeBarSet *barset = new DeclarativeBarSet(this);
    barset->setLabel(label);
    barset->setValues(values);
    if (QPercentBarSeries::insert(index, barset))
        return barset;
    delete barset;
    return 0;
}

#include "moc_declarativebarseries.cpp"

QTCOMMERCIALCHART_END_NAMESPACE
