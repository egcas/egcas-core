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

#ifndef EGCABSTRACTITEM_H
#define EGCABSTRACTITEM_H

class QRectF;
class QPointF;
class QSizeF;
class EgCasScene;

class EgcAbstractItem
{
public:
        ///std constructor
        EgcAbstractItem();
        /**
         * @brief ~EgcAbstractItem virtual destructor in order to be able to delete subclasses
         */
        virtual ~EgcAbstractItem() {}
protected:
        /**
         * @brief getEgcScene needs to be implemented by the subclasses since we cannot inherit from QGraphicsitem (the
         * subclasses already inherit from it - and we don't want to make it complicated)
         * @return pointer to EgCasScene
         */
        virtual EgCasScene* getEgcScene(void) = 0;
        /**
         * @brief bRect returns the bounding rect of the abstract item (interface to concrete item)
         * @return bounding rect rectangle
         */
        virtual QRectF bRect(void) const = 0;
        /**
         * @brief snapGrid snap to scene grid
         * @param pos the position to snap to grid
         * @return the new position that is snapped to the grid
         */
        QPointF snap(const QPointF& pos);
        /**
         * @brief getPos return the current position of the item
         * @return the current position of the item
         */
        virtual QPointF getPos(void) = 0;
        /**
         * @brief ensureVisibility does everything that needs to be done to ensure visibility of the current item
         */
        void ensureVisibility(void);
};

#endif // EGCABSTRACTITEM_H
