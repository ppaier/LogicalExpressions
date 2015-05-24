// LogicalExpressions.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "LogicalExpression.h"

#include <vector>
#include <iostream>
#include <string>


int main(int argc, char* argv[])
{
    // define where to find certain features
    CTerm<double> minX  = CTerm<double>::CreateVariableTerm(0);
    CTerm<double> minY  = CTerm<double>::CreateVariableTerm(1);
    CTerm<double> sizeX = CTerm<double>::CreateVariableTerm(2);
    CTerm<double> sizeY = CTerm<double>::CreateVariableTerm(3);

    // define CoG feature
    CTerm<double> cX = minX + sizeX / 2.0;
    CTerm<double> cY = minY + sizeY / 2.0;

    // define the logical expression
    CLogicalExpression<double> exp1 = cX > 10.0;
    CLogicalExpression<double> exp2 = cY > 10.0;
    CLogicalExpression<double> exp = exp1 && exp2; // CoG must be after row 10 and column 10

    // just some examples how to define any new feature
    // combine features using a certain function
    CTerm<double> newFeat(cX, cY, [](double a, double b) { return 3 * a + 5 * b; });

    // the same feature, but this time as a result of operator overloading
    CTerm<double> newFeat2 = 3.0*cX + 5.0*cY;

    // for more complex combinations operators are not sufficient
    CTerm<double> newFeat3(newFeat, newFeat2, [](double a, double b) { return a*a + b*b; });

    // now an indicator atom to select a feature
    CTerm<double> indicator(sizeX, sizeY, [](double a, double b) {
        if (a>b)
            return 1.0;
        else
            return 0.0;
    });

    // the value of newFeat4 is now either minX or minY, depending if sizeX > sizeY
    CTerm<double> newFeat4 = indicator * minX + (1.0 - indicator) * minY;

    std::vector<double> values1 = { 1, 2, 21, 21 };
    std::vector<double> values2 = { 3, 6, 5.1, 5 };
    std::vector<std::vector<double>> valuesVec = {values1, values2};

    std::vector<CTerm<double>> atoms = {cX, cY, newFeat, newFeat2, newFeat3, newFeat4};
    std::vector<CLogicalExpression<double>> expressions = {exp, exp1, exp2};

    std::vector<std::vector<double>> substResults = substitute(valuesVec, atoms);
    std::vector<std::vector<bool>> evalResults = evaluate(valuesVec, expressions);

    std::cout << "Results for values1" << std::endl;
    std::cout << "Evaluate exp: " << evalResults[0][0] << std::endl;
    std::cout << "Evaluate exp1: " << evalResults[0][1] << std::endl;
    std::cout << "Evaluate exp2: " << evalResults[0][2] << std::endl;
    std::cout << "cx value: " << substResults[0][0] << std::endl;
    std::cout << "cy value: " << substResults[0][1] << std::endl;
    std::cout << "New Feat value: " << substResults[0][2] << std::endl;
    std::cout << "New Feat 2 value: " << substResults[0][3] << std::endl;
    std::cout << "New Feat 3 value: " << substResults[0][4] << std::endl;
    std::cout << "New Feat 4 value: " << substResults[0][5] << std::endl;

    std::cout << "Results for values2" << std::endl;
    std::cout << "Evaluate exp: " << evalResults[1][0] << std::endl;
    std::cout << "Evaluate exp1: " << evalResults[1][1] << std::endl;
    std::cout << "Evaluate exp2: " << evalResults[1][2] << std::endl;
    std::cout << "cx value: " << substResults[1][0] << std::endl;
    std::cout << "cy value: " << substResults[1][1] << std::endl;
    std::cout << "New Feat value: " << substResults[1][2] << std::endl;
    std::cout << "New Feat 2 value: " << substResults[1][3] << std::endl;
    std::cout << "New Feat 3 value: " << substResults[1][4] << std::endl;
    std::cout << "New Feat 4 value: " << substResults[1][5] << std::endl;

	return 0;
}

