/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
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

#ifndef EGCENTITY_H
#define EGCENTITY_H

class QPointF;


/**
 * @brief The EgcEntityType defines different types of entities inside a egCAS (formula) document/worksheet
 */
enum class EgcEntityType
{
        Formula = 0,        ///< this is a formula int the document
        Picture,            ///< this is a picture in the document
        Text                ///< this is a text element in the document
};

/**
 * @brief The EgcEntity class is a abstract base class for all types of entities in a document (text, formulas, pictures, ...)
 */
class EgcEntity
{
public:
        /**
         * @brief EgcEntity std constructor
         */
        EgcEntity(void);
        ///std destructor
        virtual ~EgcEntity();
        /**
         * @brief getEntityType returns the entity type of the current class, needs to be reimplemented in a subclass
         * @return the entity type
         */
        virtual enum EgcEntityType getEntityType(void) const = 0;
        /**
         * @brief getPositon returns the position of the current entity
         * @return the position of the entity in the current worksheet
         */
        virtual QPointF getPositon(void) const = 0;
private:
};

#endif // EGCENTITY_H