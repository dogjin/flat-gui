#include "TimeEditor.h"
#include "AbstractField.h"
#include "TextDataModel.h"
#include "ToolBar.h"
#include "PushButton.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QLocale>

/*!
 * \class TimeEditor
 * \inmodule FlatGui
 */

TimeEditor::TimeEditor(QWidget *parent) :
	AbstractEditor(parent, Qt::FramelessWindowHint | Qt::Popup),
	m_labTime(new QLabel(this)),
	m_sldHours(new QSlider(this)),
	m_sldMinutes(new QSlider(this))
{
	auto *layoutMain = new QVBoxLayout(this);
	auto *layoutControls = new QVBoxLayout();
	auto *labHours = new QLabel(tr("Hours"), this);
	auto *labMinutes = new QLabel(tr("Minutes"), this);
	auto *toolbar = new ToolBar(this);
	auto *btnAccept = new PushButton(this);
	auto *btnCancel = new PushButton(this);

	m_labTime->setAlignment(Qt::AlignCenter);
	m_labTime->setFixedHeight(48);
	m_labTime->setStyleSheet(".QLabel { border: none; border-bottom: 1px solid palette(midlight); }");

	m_sldHours->setOrientation(Qt::Horizontal);
	m_sldHours->setMaximum(23);
	m_sldHours->setTickPosition(QSlider::TicksBelow);

	m_sldMinutes->setOrientation(Qt::Horizontal);
	m_sldMinutes->setMaximum(59);
	m_sldMinutes->setTickPosition(QSlider::TicksBelow);

	layoutControls->addWidget(m_labTime);
	layoutControls->addSpacing(16);
	layoutControls->addWidget(labHours);
	layoutControls->addWidget(m_sldHours);
	layoutControls->addSpacing(16);
	layoutControls->addWidget(labMinutes);
	layoutControls->addWidget(m_sldMinutes);
	layoutControls->setContentsMargins(9, 0, 9, 0);
	layoutMain->setSpacing(0);

	btnCancel->setText(tr("Cancel"));
	btnAccept->setText(tr("Apply"));

	toolbar->addWidget(btnCancel);
	toolbar->addStretch();
	toolbar->addWidget(btnAccept);
	toolbar->setFixedHeight(48);

	layoutMain->addLayout(layoutControls);
	layoutMain->addStretch();
	layoutMain->addWidget(toolbar);
	layoutMain->setContentsMargins(1, 1, 1, 1);
	layoutMain->setSpacing(0);

	setAttribute(Qt::WA_DeleteOnClose);
	setAttribute(Qt::WA_StyledBackground);
	setFixedSize(350, 250);
	setStyleSheet(".TimeEditor { border: 1px solid palette(mid); }");

	connect(m_sldHours, &QSlider::valueChanged, this, &TimeEditor::updateTime);
	connect(m_sldMinutes, &QSlider::valueChanged, this, &TimeEditor::updateTime);
	connect(btnCancel, &PushButton::clicked, this, &TimeEditor::onCancel);
	connect(btnAccept, &PushButton::clicked, this, &TimeEditor::onAccept);
}

QTime TimeEditor::time() const
{
	return QLocale().toTime(m_labTime->text(), "HH:mm");
}

void TimeEditor::setTime(const QTime &time)
{
	const QTime &t(time.isValid() ? time : QDateTime::currentDateTime().time());

	m_labTime->setText(t.toString("HH:mm"));
	m_sldHours->setValue(t.hour());
	m_sldMinutes->setValue(t.minute());
}

void TimeEditor::reposition()
{
	auto *field = this->field();

	if (!field)
		return;

	resize(field->width(), 300);
	move(field->mapToGlobal(field->rect().bottomLeft()) += QPoint(0, 3));
}

void TimeEditor::updateTime()
{
	m_labTime->setText(QString("%1").arg(m_sldHours->value(), 2, 10, QChar('0'))
					 + ":" + QString("%1").arg(m_sldMinutes->value(), 2, 10, QChar('0')));
}

void TimeEditor::onCancel()
{
	editingFinished(false);
}

void TimeEditor::onAccept()
{
	editingFinished(true);
}