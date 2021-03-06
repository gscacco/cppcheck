/*
 * Cppcheck - A tool for static C/C++ code analysis
 * Copyright (C) 2007-2009 Daniel Marjamäki and Cppcheck team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */


//---------------------------------------------------------------------------
#ifndef CheckAutoVariablesH
#define CheckAutoVariablesH
//---------------------------------------------------------------------------

#include "check.h"
#include "token.h"
#include <list>

/// @addtogroup Checks
/// @{


class CheckAutoVariables : public Check
{
public:
    /** This constructor is used when registering the CheckClass */
    CheckAutoVariables() : Check()
    { }

    /** This constructor is used when running checks.. */
    CheckAutoVariables(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
            : Check(tokenizer, settings, errorLogger)
    { }

    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
    {
        CheckAutoVariables checkAutoVariables(tokenizer, settings, errorLogger);
        checkAutoVariables.autoVariables();
        checkAutoVariables.returnPointerToLocalArray();
    }

    /** Check auto variables */
    void autoVariables();

    /** Returning pointer to local array */
    void returnPointerToLocalArray();

private:
    std::list<std::string> fp_list;
    std::list<std::string> vd_list;
    std::list<std::string> vda_list;
    bool errorAv(const Token* left, const Token* right);
    bool isAutoVar(const Token* t);
    bool isAutoVarArray(const Token* t);
    void addVD(const Token* t);
    void addVDA(const Token* t);




    void errorReturnPointerToLocalArray(const Token *tok);


    void getErrorMessages()
    {
        reportError(0, Severity::error, "autoVariables", "Wrong assignement of an auto-variable to an effective parameter of a function");
        errorReturnPointerToLocalArray(0);
    }

    std::string name() const
    {
        return "Auto Variables";
    }

    std::string classInfo() const
    {
        return "A pointer to a variable is only valid as long as the variable is in scope.\n"
               "Check:\n"
               " * returning a pointer to variable\n"
               " * assigning address of an variable to an effective parameter of a function\n";
    }
};
/// @}
//---------------------------------------------------------------------------
#endif

