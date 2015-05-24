// LogicalExpressions.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "LogicalAtom.h"
#include "LogicalExpression.h"

#include <vector>
#include <iostream>
#include <string>


int main(int argc, char** argv[])
{		
    // define where to find certain features
    CLogicalAtom<double> minX  = CLogicalAtom<double>::CreateVarAtom(0);
    CLogicalAtom<double> minY  = CLogicalAtom<double>::CreateVarAtom(1);
    CLogicalAtom<double> sizeX = CLogicalAtom<double>::CreateVarAtom(2);
    CLogicalAtom<double> sizeY = CLogicalAtom<double>::CreateVarAtom(3);

    // define CoG feature
    CLogicalAtom<double> cX = minX + sizeX / 2.0;
    CLogicalAtom<double> cY = minY + sizeY / 2.0;

    // define the logical expression
    CLogicalExpression<double> exp1 = cX > 10.0;
    CLogicalExpression<double> exp2 = cY > 10.0;
    CLogicalExpression<double> exp = exp1 && exp2; // CoG must be after row 10 and column 10

    // just some examples how to define any new feature
    // combine features using a certain function
    CLogicalAtom<double> newFeat(cX, cY, [](double a, double b) { return 3 * a + 5 * b; });

    // the same feature, but this time as a result of operator overloading
    CLogicalAtom<double> newFeat2 = 3.0*cX + 5.0*cY;

    // for more complex combinations operators are not sufficient
    CLogicalAtom<double> newFeat3(newFeat, newFeat2, [](double a, double b) { return a*a + b*b; });

    // now an indicator atom to select a feature
    CLogicalAtom<double> indicator(sizeX, sizeY, [](double a, double b) {
        if (a>b)
            return 1.0;
        else
            return 0.0;
    });

    // the value of newFeat4 is now either minX or minY, depending if sizeX > sizeY
    CLogicalAtom<double> newFeat4 = indicator * minX + (1.0 - indicator) * minY;


    // now evaluate logical expression for different values and calc new feature values
    std::vector<double> values1 = { 1, 2, 21, 21 };

    std::cout << "Evaluate exp for values1: " << exp.evaluate(values1) << std::endl;
    std::cout << "cx value: " << cX.subst(values1) << std::endl;
    std::cout << "cy value: " << cY.subst(values1) << std::endl;
    std::cout << "New Feat value: " << newFeat.subst(values1) << std::endl;
    std::cout << "New Feat 2 value: " << newFeat2.subst(values1) << std::endl;
    std::cout << "New Feat 3 value: " << newFeat3.subst(values1) << std::endl;
    std::cout << "New Feat 4 value: " << newFeat4.subst(values1) << std::endl;

    std::vector<double> values2 = { 3, 6, 5.1, 5 };

    std::cout << "Evaluate exp for values2: " << exp.evaluate(values2) << std::endl;
    std::cout << "cx value: " << cX.subst(values2) << std::endl;
    std::cout << "cy value: " << cY.subst(values2) << std::endl;
    std::cout << "New Feat value: " << newFeat.subst(values2) << std::endl;
    std::cout << "New Feat 2 value: " << newFeat2.subst(values2) << std::endl;
    std::cout << "New Feat 3 value: " << newFeat3.subst(values2) << std::endl;
    std::cout << "New Feat 4 value: " << newFeat4.subst(values2) << std::endl;
	return 0;
}

