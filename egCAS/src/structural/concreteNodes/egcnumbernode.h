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

#ifndef EGCNUMBERNODE_H
#define EGCNUMBERNODE_H

#include <QtGlobal>
#include <QString>
#include <QRegularExpression>
#include "../specialNodes/egcnode.h"

/**
 * @brief The EgcNumberNode class is a class that holds leafes with numbers
 */
class EgcNumberNode : public EgcNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcNumberNode, EgcNodeType::NumberNode);

public:
        ///std constructor
        EgcNumberNode();
        ///destructor
        virtual ~EgcNumberNode();
        /**
         * @brief setValue set the value of this leaf
         * @param value the number value to be set
         */
        void setValue(const QString& value);
        /**
         * @brief getValue returns the value saved in this class
         * @return the value of this object
         */
        QString getValue(void) const;
        /**
         * @brief operator== comparison operator overload
         * @param node the node to compare against
         * @return true if the trees are equal
         */
        virtual bool operator==(const EgcNode& node) const override;
        /**
         * @brief nrSubindexes returns the number of subindexes of this node. This can be e.g. the number of characters
         * of a number or variable
         * @return 0 if not overwritten, since a std. node has no subindexes
         */
        virtual int nrSubindexes(void) const override;        
        /**
         * @brief interface for serializing the attributes of a formula operation
         * @param stream the stream to use for serializing this class
         */
        virtual void serializeAttributes(QXmlStreamWriter& stream) override;

        /**
         * @brief deserialize interface for deserializing the attributes of a formula operation
         * @param stream the xml reader stream
         * @param version the version of the stream that is to be deserialized
         * @param attr the xml attributes provided by the parent
         */
        virtual void deserializeAttributes(QXmlStreamReader& stream, quint32 version, QXmlStreamAttributes& attr) override;
        /**
         * @brief interface for serializing a class
         * @param stream the stream to use for serializing this class
         * @param properties object with all neccessary information for serializing
         */
        virtual void serialize(QXmlStreamWriter& stream, SerializerProperties& properties) override;
        /**
         * @brief deserialize interface for deserializing a class
         * @param stream the stream to use for deserializing this class
         * @param properties object with all neccessary information for deserializing
         */
        virtual void deserialize(QXmlStreamReader& stream, SerializerProperties &properties) override;


protected:
        /**
         * @brief insert insert a character at the given position
         * @param character the character to insert
         * @param position the position where to insert the given character
         * @return true if the operation was successful, false otherwise
         */
        virtual bool insert(QChar character, int position);
        /**
         * @brief remove a charcter at the given position
         * @param position the position at which to remove a character
         * @return true if the operation was successful, false otherwise
         */
        virtual bool remove(int position);

private:
        QString m_value;                                ///< the value of the number
        static QRegularExpression s_validator;          ///< a validator for character inputs
        static bool s_regexInitialized;                 ///< is the regex already initialized
};

#endif // EGCNUMBERNODE_H
