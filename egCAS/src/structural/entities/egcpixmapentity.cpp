/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of egCAS nor the names of its
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

#include <QFont>
#include <QSizeF>
#include <QByteArray>
#include <QString>
#include <QBuffer>
#include "egcpixmapentity.h"
#include "egcabstractpixmapitem.h"

EgcPixmapEntity::EgcPixmapEntity(void) : m_item(nullptr)
{
}

EgcPixmapEntity::~EgcPixmapEntity()
{
}

EgcEntityType EgcPixmapEntity::getEntityType(void) const
{
        return EgcEntityType::Text;
}

QPointF EgcPixmapEntity::getPositon(void) const
{
        if (m_item)
                return m_item->getPosition();
        else
                return QPointF(0.0,0.0);
}

QString EgcPixmapEntity::getFilePath(void) const
{
        if (m_isEmbedded)
                return nullptr;
        else
                return m_path;
}

QByteArray EgcPixmapEntity::getB64Encoded(void) const
{
        if (!m_item)
                return QByteArray();

        //write pixmap in bytes as png format
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        m_item->getPixmap().save(&buffer, "PNG");

        return bytes.toBase64();
}

QSizeF EgcPixmapEntity::getSize(void) const
{
        if (!m_item)
                return QSizeF(0.0, 0.0);

        return m_item->getSize();
}

void EgcPixmapEntity::setSize(QSizeF size)
{
        if (!m_item)
                return;

        QSizeF tmp = m_item->getSize();
        qreal xFactor = size.width()/tmp.width();
        qreal yFactor = size.height()/tmp.height();
        qreal factor = qMin(xFactor, yFactor);
        m_item->setScaleFactor(factor);
}

void EgcPixmapEntity::setItem(EgcAbstractPixmapItem* item)
{
        m_item = item;
}

void EgcPixmapEntity::setPosition(QPointF pos)
{
        if (!m_item)
                return;

        m_item->setPos(pos);
}

void EgcPixmapEntity::setFilePath(QString file)
{
        if (!m_item)
                return;

        m_path = file;
        m_item->setPixmap(QPixmap(file));
}

void EgcPixmapEntity::itemChanged(EgcItemChangeType changeType)
{

}
