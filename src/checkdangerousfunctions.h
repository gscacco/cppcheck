/*
 * Cppcheck - A tool for static C/C++ code analysis
 * Copyright (C) 2007-2009 Daniel Marjamäki, Reijo Tomperi, Nicolas Le Cam,
 * Leandro Penz, Kimmo Varis
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
#ifndef CheckDangerousFunctionsH
#define CheckDangerousFunctionsH
//---------------------------------------------------------------------------

#include "tokenize.h"
#include "errorlogger.h"

class CheckDangerousFunctionsClass
{
public:
    CheckDangerousFunctionsClass(const Tokenizer *tokenizer, const Settings &settings, ErrorLogger *errorLogger);
    ~CheckDangerousFunctionsClass();

    /** Check for buffer overruns */
    void dangerousFunctions();

private:

    const Tokenizer *_tokenizer;
    const Settings _settings;
    ErrorLogger *_errorLogger;
};

//---------------------------------------------------------------------------
#endif
