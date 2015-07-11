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

#include <QString>
#include <QtTest>
#include <iostream>
#include <QScopedPointer>
#include "egckernelparser.h"
#include "egcnodes.h"
#include "egcnodeiterator.h"
#include "egcformulaexpression.h"
#include "egcnodecreator.h"
#include "egcnodevisitor.h"
#include "egcmaximavisitor.h"
#include "egcmathmlvisitor.h"
#include "casKernel/egcmaximaconn.h"
#include "egcformulaexpression.h"


#include <QDebug>

class EgcasTest_Calculation : public QObject
{
        Q_OBJECT

public:
        EgcasTest_Calculation() : hasEnded(false) {}
public Q_SLOTS:
        void evaluateResult(QString result);
        void kernelStarted();
private Q_SLOTS:
        void basicTestCalculation();
private:
        EgcNode* getTree(QString formula);
        QScopedPointer<EgcMaximaConn> conn;
        EgcFormulaExpression formula;
        EgcKernelParser parser;
        bool hasEnded;
};

EgcNode* EgcasTest_Calculation::getTree(QString formula)
{
        QScopedPointer<EgcNode> tree(parser.parseKernelOutput(formula));
        if (tree.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();
        }

        return tree.take();
}

void EgcasTest_Calculation::basicTestCalculation()
{                
        conn.reset(new (std::nothrow) EgcMaximaConn("/usr/bin/maxima", this));
        connect(conn.data(), SIGNAL(resultReceived(QString)), this, SLOT(evaluateResult(QString)));
        connect(conn.data(), SIGNAL(kernelStarted()), this, SLOT(kernelStarted()));
        QVERIFY(!conn.isNull());
        do {
                QTest::qWait(100);
        } while (!hasEnded);
        conn->quit();
        QTest::qWait(500);
}

void EgcasTest_Calculation::kernelStarted()
{
        formula.setRootElement(getTree("x:33.1"));
        conn->sendCommand(formula.getCASKernelCommand());
}

void EgcasTest_Calculation::evaluateResult(QString result)
{
        static int i = 0;

        //test equal functions
        QScopedPointer<EgcNode> tree1(getTree("x^3+36-8*651.984+fnc1(x)"));
        QScopedPointer<EgcNode> tree2(getTree("x^3+36-8*651.984+fnc1(x)"));
        QVERIFY(*tree1.data() == *tree2.data());

        if (i == 0) {
                QVERIFY(result == "33.1");
                formula.setRootElement(getTree("x^3+36-8*651.984"));
                conn->sendCommand(formula.getCASKernelCommand());
        } else if (i == 1) {
                QVERIFY(result == "31084.819");
                EgcFormulaExpression form_res;
                form_res.setRootElement(getTree("y=3"));
                QVERIFY(form_res.setResult(getTree(result)) == true);
                QVERIFY(form_res.getMathMlCode() == "<math><mrow><mi>y</mi><mo>=</mo><mn>31084.819</mn></mrow></math>");
                hasEnded = true;
        }

        i++;
}






QTEST_MAIN(EgcasTest_Calculation)

#include "tst_egcastest_calculations.moc"