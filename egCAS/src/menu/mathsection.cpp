/*
Copyright (c) 2015, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the egCAS nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#include <QVBoxLayout>
#include <QPushButton>
#include <QSignalMapper>
#include "mathsection.h"

MathSection::MathSection(QWidget *parent) : QWidget(parent), m_nrCoulumns{7}, m_index{0}
{
        bool ass_ret;

        m_section = new CollapsableSectionWidget(CollapsableSectionWidget::CollapsableSectionLayout::grid, this);
        m_signalMapper = new QSignalMapper(this);

        QVBoxLayout* vLayout = new QVBoxLayout(this);
        vLayout->setSpacing(0);
        vLayout->setContentsMargins(0, 0, 0, 0);
        vLayout->addWidget(m_section);

        qRegisterMetaType<ActionWrapper>("ActionWrapper");
        ass_ret = connect(m_signalMapper, SIGNAL(mapped(QObject*)), this, SLOT(clicked(QObject*)));
        Q_ASSERT(ass_ret == true);
}

void MathSection::clicked(QObject* action)
{
        ActionWrapper* act = dynamic_cast<ActionWrapper*>(action);
        if (act)
                emit actionTriggered(act->getAction());
}

void MathSection::setText(const QString & text)
{
        m_section->setText(text);
}

void MathSection::setChecked(void)
{
        m_section->setChecked(true);
}

void MathSection::setNrColumns(quint32 columns)
{
        m_nrCoulumns = columns;
}

void MathSection::addElement(MathElement element)
{
        QPushButton *btn;
        bool retval;

        btn = new QPushButton(m_section);
        btn->setStyleSheet("padding:0px;background-color:rgb(64, 66, 68);");
        btn->setFlat(true);
        if (element.m_isIcon) {
                QPixmap pix = QPixmap(element.m_designator);
                QSize size = pix.size();
                btn->setIcon(QIcon(element.m_designator));
                btn->setIconSize(size);
        } else {
                btn->setText(element.m_designator);
        }
        btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        retval = connect(btn, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
        Q_ASSERT(retval == true);
        ActionWrapper* wrapper = new (std::nothrow) ActionWrapper(element.m_action, this);
        m_signalMapper->setMapping(btn, wrapper);
        m_section->addWidget(btn, m_index / m_nrCoulumns, m_index % m_nrCoulumns, 1, 1);
        m_index++;
}

void MathSection::newRow(void)
{
        m_index += m_nrCoulumns - (m_index % m_nrCoulumns);
}
