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

#ifndef EGCABSTRACTDOCUMENT_H
#define EGCABSTRACTDOCUMENT_H


class EgcAbstractFormulaEntity;
class EgcEntity;
class QGraphicsItem;
class QSizeF;
class QPointF;

class EgcAbstractDocument 
{
public:
        ///destructor
        virtual ~EgcAbstractDocument() {}
        /**
         * @brief deleteFormula delete a formula
         * @param formula the formula to delete
         * @return true if everything went well, false otherwise
         */
        virtual bool formulaEntityDeleted(EgcEntity* formula) = 0;
        /**
         * @brief deleteEntity Delete the given entity. This can be called from the entity itself.
         * @param entity the entity to delete
         */
        virtual void deleteEntity(EgcEntity* entity) = 0;
        /**
         * @brief resumeCalculation resume calculation after it has been stopped due to e.g. editing a formula.
         */
        virtual void resumeCalculation(void) = 0;
        /**
         * @brief startCalulation start the calculation of the document in the background
         * @param entity the entity where to pause calculation
         */
        virtual void startCalulation(EgcAbstractFormulaEntity* entity) = 0;
        /**
         * @brief restartCalculation starts the calculation from the beginning of the document, e.g. after position
         * change of a formula.
         */
        virtual void restartCalculation(void) = 0;
        /**
         * @brief getMaxSize get the maximum size a rectangular item can have with the given starting point in order
         * to fit into the current worksheet
         * @param point the point at which the item should be
         * @return the maximum size the item can have
         */
        virtual QSizeF getMaxItemSize(QPointF point) const = 0;
        /**
         * @brief itemDeleted is called by the scene if an item has been deleted
         * @param item the item that has been deleted
         */
        virtual void itemDeleted(QGraphicsItem* item) = 0;

};

#endif //#ifndef EGCABSTRACTDOCUMENT_H
