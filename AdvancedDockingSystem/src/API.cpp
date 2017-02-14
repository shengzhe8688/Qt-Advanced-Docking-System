#include <ads/MainContainerWidget.h>
#include "ads/API.h"

#include <QWidget>
#include <QSplitter>
#include <QLayout>
#include <QVariant>

#include "ads/SectionWidget.h"

ADS_NAMESPACE_BEGIN

static bool splitterContainsSectionWidget(QSplitter* splitter)
{
	for (int i = 0; i < splitter->count(); ++i)
	{
		QWidget* w = splitter->widget(i);
		QSplitter* sp = qobject_cast<QSplitter*>(w);
		SectionWidget* sw = NULL;
		if (sp && splitterContainsSectionWidget(sp))
			return true;
		else if ((sw = qobject_cast<SectionWidget*>(w)) != NULL)
			return true;
	}
	return false;
}

void deleteEmptySplitter(MainContainerWidget* container)
{
	bool doAgain = false;
	do
	{
		doAgain = false;
		QList<QSplitter*> splitters = container->findChildren<QSplitter*>();
		for (int i = 0; i < splitters.count(); ++i)
		{
			QSplitter* sp = splitters.at(i);
			if (!sp->property("ads-splitter").toBool())
				continue;
			if (sp->count() > 0 && splitterContainsSectionWidget(sp))
				continue;
			delete splitters[i];
			doAgain = true;
			break;
		}
	}
	while (doAgain);
}

MainContainerWidget* findParentContainerWidget(QWidget* w)
{
	MainContainerWidget* cw = 0;
	QWidget* next = w;
	do
	{
		if ((cw = dynamic_cast<MainContainerWidget*>(next)) != 0)
		{
			break;
		}
		next = next->parentWidget();
	}
	while (next);
	return cw;
}

SectionWidget* findParentSectionWidget(class QWidget* w)
{
	SectionWidget* cw = 0;
	QWidget* next = w;
	do
	{
		if ((cw = dynamic_cast<SectionWidget*>(next)) != 0)
		{
			break;
		}
		next = next->parentWidget();
	}
	while (next);
	return cw;
}

QSplitter* findParentSplitter(class QWidget* w)
{
	QSplitter* cw = 0;
	QWidget* next = w;
	do
	{
		if ((cw = dynamic_cast<QSplitter*>(next)) != 0)
		{
			break;
		}
		next = next->parentWidget();
	}
	while (next);
	return cw;
}

QSplitter* findImmediateSplitter(class QWidget* w)
{
	QLayout* l = w->layout();
	if (!l || l->count() <= 0)
	{
		return nullptr;
	}

	QSplitter* sp = nullptr;
	for (int i = 0; i < l->count(); ++i)
	{
		QLayoutItem* li = l->itemAt(0);
		if (!li->widget())
		{
			continue;
		}
		if ((sp = dynamic_cast<QSplitter*>(li->widget())) != nullptr)
		{
			break;
		}
	}
	return sp;
}

ADS_NAMESPACE_END
