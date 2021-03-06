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


#include "checkmemoryleak.h"
#include "mathlib.h"
#include "tokenize.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <set>

//---------------------------------------------------------------------------

// Register this check class (by creating a static instance of it)
namespace
{
CheckMemoryLeakInFunction instance1;
CheckMemoryLeakInClass instance2;
CheckMemoryLeakStructMember instance3;
}

//---------------------------------------------------------------------------

bool CheckMemoryLeak::isclass(const Tokenizer *_tokenizer, const Token *tok) const
{
    if (tok->isStandardType())
        return false;

    std::ostringstream pattern;
    pattern << "struct " << tok->str();
    if (Token::findmatch(_tokenizer->tokens(), pattern.str().c_str()))
        return false;

    return true;
}
//---------------------------------------------------------------------------

CheckMemoryLeak::AllocType CheckMemoryLeak::getAllocationType(const Token *tok2) const
{
    // What we may have...
    //     * var = (char *)malloc(10);
    //     * var = new char[10];
    //     * var = strdup("hello");
    //     * var = strndup("hello", 3);
    if (tok2 && tok2->str() == "(")
    {
        while (tok2 && tok2->str() != ")")
            tok2 = tok2->next();
        tok2 = tok2 ? tok2->next() : NULL;
    }
    if (! tok2)
        return No;
    if (! tok2->isName())
        return No;

    // Does tok2 point on "malloc", "strdup" or "kmalloc"..
    static const char * const mallocfunc[] = {"malloc",
            "calloc",
            "strdup",
            "strndup",
            "kmalloc",
            "kzalloc",
            "kcalloc",
            0
                                             };
    for (unsigned int i = 0; mallocfunc[i]; i++)
    {
        if (tok2->str() == mallocfunc[i])
            return Malloc;
    }

    // Does tok2 point on "g_malloc", "g_strdup", ..
    static const char * const gmallocfunc[] = {"g_new",
            "g_new0",
            "g_try_new",
            "g_try_new0",
            "g_malloc",
            "g_malloc0",
            "g_try_malloc",
            "g_try_malloc0",
            "g_strdup",
            "g_strndup",
            0
                                              };
    for (unsigned int i = 0; gmallocfunc[i]; i++)
    {
        if (tok2->str() == gmallocfunc[i])
            return gMalloc;
    }

    if (Token::Match(tok2, "new %type% [;()]") ||
        Token::Match(tok2, "new ( std :: nothrow ) %type% [;()]") ||
        Token::Match(tok2, "new ( nothrow ) %type% [;()]"))
        return New;

    if (Token::Match(tok2, "new %type% [") ||
        Token::Match(tok2, "new ( std :: nothrow ) %type% [") ||
        Token::Match(tok2, "new ( nothrow ) %type% ["))
        return NewArray;

    if (Token::Match(tok2, "fopen|tmpfile ("))
        return File;

    if (Token::Match(tok2, "open|openat|creat|mkstemp|mkostemp ("))
        return Fd;

    if (Token::simpleMatch(tok2, "popen ("))
        return Pipe;

    if (Token::Match(tok2, "opendir|fdopendir ("))
        return Dir;

    return No;
}



CheckMemoryLeak::AllocType CheckMemoryLeak::getReallocationType(const Token *tok2)
{
    // What we may have...
    //     * var = (char *)realloc(..;
    if (tok2 && tok2->str() == "(")
    {
        while (tok2 && tok2->str() != ")")
            tok2 = tok2->next();
        tok2 = tok2 ? tok2->next() : NULL;
    }
    if (! tok2)
        return No;

    if (tok2->str() == "realloc")
        return Malloc;

    // GTK memory reallocation..
    if (Token::Match(tok2, "g_realloc|g_try_realloc|g_renew|g_try_renew"))
        return gMalloc;

    return No;
}


CheckMemoryLeak::AllocType CheckMemoryLeak::getDeallocationType(const Token *tok, const char *varnames[])
{
    int i = 0;
    std::string names;
    while (varnames[i])
    {
        if (i > 0)
            names += " . ";

        names += varnames[i];
        i++;
    }

    if (Token::simpleMatch(tok, std::string("delete " + names + " ;").c_str()))
        return New;

    if (Token::simpleMatch(tok, std::string("delete [ ] " + names + " ;").c_str()))
        return NewArray;

    if (Token::simpleMatch(tok, std::string("delete ( " + names + " ) ;").c_str()))
        return New;

    if (Token::simpleMatch(tok, std::string("delete [ ] ( " + names + " ) ;").c_str()))
        return NewArray;

    if (Token::simpleMatch(tok, std::string("free ( " + names + " ) ;").c_str()) ||
        Token::simpleMatch(tok, std::string("kfree ( " + names + " ) ;").c_str()))
        return Malloc;

    if (Token::simpleMatch(tok, std::string("g_free ( " + names + " ) ;").c_str()))
        return gMalloc;

    if (Token::simpleMatch(tok, std::string("fclose ( " + names + " )").c_str()) ||
        Token::simpleMatch(tok, "fcloseall ( )"))
        return File;

    if (Token::simpleMatch(tok, std::string("close ( " + names + " )").c_str()))
        return Fd;

    if (Token::simpleMatch(tok, std::string("pclose ( " + names + " )").c_str()))
        return Pipe;

    if (Token::simpleMatch(tok, std::string("closedir ( " + names + " )").c_str()))
        return Dir;

    return No;
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------

void CheckMemoryLeak::memoryLeak(const Token *tok, const char varname[], AllocType alloctype, bool all)
{
    if (alloctype == CheckMemoryLeak::File ||
        alloctype == CheckMemoryLeak::Pipe ||
        alloctype == CheckMemoryLeak::Fd   ||
        alloctype == CheckMemoryLeak::Dir)
        resourceLeakError(tok, varname);
    else if (all)
        memleakallError(tok, varname);
    else
        memleakError(tok, varname);
}
//---------------------------------------------------------------------------


void CheckMemoryLeak::reportErr(const Token *tok, Severity::e severity, const std::string &id, const std::string &msg) const
{
    std::list<const Token *> callstack;

    if (tok)
        callstack.push_back(tok);

    reportErr(callstack, severity, id, msg);
}

void CheckMemoryLeak::reportErr(const std::list<const Token *> &callstack, Severity::e severity, const std::string &id, const std::string &msg) const
{
    std::list<ErrorLogger::ErrorMessage::FileLocation> locations;

    for (std::list<const Token *>::const_iterator it = callstack.begin(); it != callstack.end(); ++it)
    {
        ErrorLogger::ErrorMessage::FileLocation loc;
        loc.line = (*it)->linenr();
        loc.file = tokenizer->file(*it);

        locations.push_back(loc);
    }

    const ErrorLogger::ErrorMessage errmsg(locations, Severity::stringify(severity), msg, id);

    if (errorLogger)
        errorLogger->reportErr(errmsg);
    else
        std::cout << errmsg.toXML() << std::endl;
}

void CheckMemoryLeak::memleakError(const Token *tok, const std::string &varname)
{
    reportErr(tok, Severity::error, "memleak", "Memory leak: " + varname);
}

void CheckMemoryLeak::memleakallError(const Token *tok, const std::string &varname)
{
    reportErr(tok, Severity::possibleError, "memleakall", "Memory leak: " + varname);
}

void CheckMemoryLeak::resourceLeakError(const Token *tok, const std::string &varname)
{
    reportErr(tok, Severity::error, "resourceLeak", "Resource leak: " + varname);
}

void CheckMemoryLeak::deallocDeallocError(const Token *tok, const std::string &varname)
{
    reportErr(tok, Severity::error, "deallocDealloc", "Deallocating a deallocated pointer: " + varname);
}

void CheckMemoryLeak::deallocuseError(const Token *tok, const std::string &varname)
{
    reportErr(tok, Severity::error, "deallocuse", "Dereferencing '" + varname + "' after it is deallocated / released");
}

void CheckMemoryLeak::mismatchSizeError(const Token *tok, const std::string &sz)
{
    reportErr(tok, Severity::error, "mismatchSize", "The given size " + sz + " is mismatching");
}

void CheckMemoryLeak::mismatchAllocDealloc(const std::list<const Token *> &callstack, const std::string &varname)
{
    reportErr(callstack, Severity::error, "mismatchAllocDealloc", "Mismatching allocation and deallocation: " + varname);
}

CheckMemoryLeak::AllocType CheckMemoryLeak::functionReturnType(const Token *tok) const
{
    // Locate the start of the function..
    unsigned int parlevel = 0;
    while (tok)
    {
        if (tok->str() == "{" || tok->str() == "}")
            return No;

        if (tok->str() == "(")
        {
            if (parlevel != 0)
                return No;
            ++parlevel;
        }

        else if (tok->str() == ")")
        {
            if (parlevel != 1)
                return No;
            break;
        }

        tok = tok->next();
    }

    // Is this the start of a function?
    if (!Token::Match(tok, ") const| {"))
        return No;

    while (tok->str() != "{")
        tok = tok->next();
    tok = tok ? tok->next() : 0;

    // Inspect the statements..
    std::string varname;
    AllocType allocType = No;
    while (tok)
    {
        // variable declaration..
        if (Token::Match(tok, "%type% * %var% ;"))
        {
            tok = tok->tokAt(4);
            continue;
        }

        if (varname.empty() && Token::Match(tok, "%var% = "))
        {
            varname = tok->str();
            allocType = getAllocationType(tok->tokAt(2));
            if (allocType == No)
                return No;
            while (tok && tok->str() != ";")
                tok = tok->next();
            tok = tok ? tok->next() : 0;
            continue;
        }

        if (tok->str() == "return")
        {
            if (varname.size() && Token::Match(tok->next(), (varname + " ;").c_str()))
                return allocType;
            if (Token::Match(tok, "return new %type% ;"))
                return New;
            if (Token::Match(tok, "return new %type% [ %any% ] ;"))
                return NewArray;
        }

        return No;
    }

    return No;
}



bool CheckMemoryLeakInFunction::matchFunctionsThatReturnArg(const Token *tok, const std::string &varname)
{
    return Token::Match(tok, std::string("; " + varname + " = strcat|memcpy|memmove|strcpy ( " + varname + " ,").c_str());
}


bool CheckMemoryLeakInFunction::notvar(const Token *tok, const char *varnames[], bool endpar)
{
    std::string varname;
    for (int i = 0; varnames[i]; i++)
    {
        if (i > 0)
            varname += " . ";

        varname += varnames[i];
    }

    const std::string end(endpar ? " &&|)" : " [;)&|]");

    return bool(Token::Match(tok, ("! " + varname + end).c_str()) ||
                Token::Match(tok, ("! ( " + varname + " )" + end).c_str()));
}


static int countParameters(const Token *tok)
{
    if (!Token::Match(tok, "%var% ("))
        return -1;
    if (Token::Match(tok->tokAt(2), "void| )"))
        return 0;

    int numpar = 1;
    int parlevel = 0;
    for (; tok; tok = tok->next())
    {
        if (tok->str() == "(")
            ++parlevel;

        else if (tok->str() == ")")
        {
            if (parlevel <= 1)
                return numpar;
            --parlevel;
        }

        else if (parlevel == 1 && tok->str() == ",")
            ++numpar;
    }

    return -1;
}

const char * CheckMemoryLeakInFunction::call_func(const Token *tok, std::list<const Token *> callstack, const char *varnames[], AllocType &alloctype, AllocType &dealloctype, bool &all, unsigned int sz)
{
    // Keywords that are not function calls..
    if (Token::Match(tok, "if|for|while|return|switch"))
        return 0;

    // String functions that are not allocating nor deallocating memory..
    if (Token::Match(tok, "strcpy|strncpy|strcat|strncat|strcmp|strncmp|strcasecmp|stricmp|sprintf|strchr|strrchr|strstr"))
        return 0;

    // Memory functions that are not allocating nor deallocating memory..
    if (Token::Match(tok, "memset|memcpy|memmove|memchr"))
        return 0;

    // I/O functions that are not allocating nor deallocating memory..
    if (Token::Match(tok, "fgets|fgetc|getc|fputs|fputc|feof|ferror|clearerr|printf|fprintf") ||
        Token::Match(tok, "fread|fwrite|fflush|fseek|fseeko|ftell|ftello|fsetpos|fgetpos") ||
        Token::Match(tok, "setvbuf|setbuf|setbuffer|setlinebuf|rewind"))
        return 0;

    // I/O functions that are not allocating nor deallocating memory..
    if (Token::Match(tok, "read|readv|pread|readahead|write|writev|pwrite|lseek") ||
        Token::Match(tok, "ioctl|fchmod|fcntl|flock|lockf|ftruncate|fsync|fdatasync") ||
        Token::Match(tok, "fstat|sync_file_range|posix_fallocate|posix_fadvise"))
        return 0;

    // Functions to work with directories that are not allocating nor
    // deallocating memory..
    if (Token::Match(tok, "readdir|readdir_r|rewinddir|telldir|seekdir|scandir"))
        return 0;

    // Convert functions that are not allocating nor deallocating memory..
    if (Token::Match(tok, "atoi|atof|atol|strtol|strtoul|strtod"))
        return 0;

    // This is not an unknown function neither
    if (tok->str() == "delete")
        return 0;

    if (getAllocationType(tok) != No || getReallocationType(tok) != No || getDeallocationType(tok, varnames) != No)
        return 0;

    if (callstack.size() > 2)
        return "dealloc_";

    const std::string funcname(tok->str());
    for (std::list<const Token *>::const_iterator it = callstack.begin(); it != callstack.end(); ++it)
    {
        if ((*it)->str() == funcname)
            return "recursive";
    }
    callstack.push_back(tok);

    // Check if this is a function that allocates memory..
    {
        const Token *ftok = _tokenizer->getFunctionTokenByName(funcname.c_str());
        AllocType a = functionReturnType(ftok);
        if (a != No)
            return "alloc";
    }

    // how many parameters is there in the function call?
    int numpar = countParameters(tok);
    if (numpar <= 0)
        return "callfunc";

    int par = 1;
    int parlevel = 0;
    std::string pattern = "[,()] ";
    for (int i = 0; varnames[i]; i++)
    {
        if (i > 0)
            pattern += " . ";

        pattern += varnames[i];
    }

    pattern += " [,()]";

    for (; tok; tok = tok->next())
    {
        if (tok->str() == "(")
            ++parlevel;
        else if (tok->str() == ")")
        {
            --parlevel;
            if (parlevel < 1)
            {
                return _settings->_showAll ? 0 : "callfunc";
            }
        }

        if (parlevel == 1)
        {
            if (tok->str() == ",")
                ++par;
            if (Token::Match(tok, pattern.c_str()))
            {
                const Token *ftok = _tokenizer->getFunctionTokenByName(funcname.c_str());
                if (!ftok)
                    return "use";

                // how many parameters does the function want?
                if (numpar != countParameters(ftok))
                    return "recursive";

                const char *parname = Tokenizer::getParameterName(ftok, par);
                if (! parname)
                    return "recursive";
                // Check if the function deallocates the variable..
                while (ftok && (ftok->str() != "{"))
                    ftok = ftok->next();
                Token *func = getcode(ftok->tokAt(1), callstack, parname, alloctype, dealloctype, false, all, sz);
                simplifycode(func, all);
                const Token *func_ = func;
                while (func_ && func_->str() == ";")
                    func_ = func_->next();

                const char *ret = 0;
                /** @todo handle "goto" */
                if (Token::findmatch(func_, "dealloc"))
                    ret = "dealloc";
                else if (Token::findmatch(func_, "use"))
                    ret = "use";
                else if (Token::findmatch(func_, "&use"))
                    ret = "&use";

                Tokenizer::deleteTokens(func);
                return ret;
            }
        }
    }
    return NULL;
}



Token *CheckMemoryLeakInFunction::getcode(const Token *tok, std::list<const Token *> callstack, const char varname[], AllocType &alloctype, AllocType &dealloctype, bool classmember, bool &all, unsigned int sz)
{
    const char *varnames[2];
    varnames[0] = varname;
    varnames[1] = 0;
    std::string varnameStr = varname;

    Token *rethead = 0, *rettail = 0;
#define addtoken(_str)                  \
    {                                       \
        if (rettail)                        \
        {                                   \
            rettail->insertToken(_str);     \
            rettail = rettail->next();      \
        }                                   \
        else                                \
        {                                   \
            rethead = new Token;            \
            rettail = rethead;              \
            rettail->str(_str);             \
        }                                   \
                                            \
        rettail->linenr( tok->linenr() );   \
        rettail->fileIndex( tok->fileIndex() ); \
    }

    // The first token should be ";"
    addtoken(";");

    bool isloop = false;

    int indentlevel = 0;
    int parlevel = 0;
    for (; tok; tok = tok->next())
    {
        if (tok->str() == "{")
        {
            addtoken("{");
            ++indentlevel;
        }
        else if (tok->str() == "}")
        {
            addtoken("}");
            if (indentlevel <= 0)
                break;
            --indentlevel;
        }

        if (tok->str() == "(")
            ++parlevel;
        else if (tok->str() == ")")
            --parlevel;
        isloop &= (parlevel > 0);

        if (parlevel == 0 && tok->str() == ";")
            addtoken(";");

        if (Token::Match(tok->previous(), std::string("[(;{}] " + varnameStr + " =").c_str()))
        {
            AllocType alloc = getAllocationType(tok->tokAt(2));
            bool realloc = false;

            if (sz > 1 &&
                Token::Match(tok->tokAt(2), "malloc ( %num% )") &&
                (MathLib::toLongNumber(tok->strAt(4)) % sz) != 0)
            {
                mismatchSizeError(tok->tokAt(4), tok->strAt(4));
            }

            if (alloc == No)
            {
                alloc = getReallocationType(tok->tokAt(2));
                if (alloc != No)
                {
                    addtoken("realloc");
                    addtoken(";");
                    realloc = true;
                    tok = tok->tokAt(2);
                    continue;
                }
            }

            // If "--all" hasn't been given, don't check classes..
            if (alloc == New)
            {
                if (Token::Match(tok->tokAt(2), "new %type% [(;]"))
                {
                    if (isclass(_tokenizer, tok->tokAt(3)))
                    {
                        if (_settings->_showAll)
                        {

                            if (_settings->isAutoDealloc(tok->strAt(3)))
                            {
                                // This class has automatic deallocation
                                alloc = No;
                            }
                            else
                            {
                                // The checking will proceed.. but any error messages that are shown are shown thanks to "--all"
                                all = true;
                            }
                        }
                        else
                            alloc = No;
                    }
                }
            }

            if (alloc != No)
            {
                if (! realloc)
                    addtoken("alloc");

                if (alloctype != No && alloctype != alloc)
                    alloc = Many;

                if (alloc != Many && dealloctype != No && dealloctype != Many && dealloctype != alloc)
                {
                    callstack.push_back(tok);
                    mismatchAllocDealloc(callstack, varname);
                    callstack.pop_back();
                }

                alloctype = alloc;
            }

            else if (matchFunctionsThatReturnArg(tok->previous(), std::string(varname)))
            {
                addtoken("use");
            }

            // assignment..
            else
            {
                // is the pointer in rhs?
                bool rhs = false;
                std::string pattern("[=+] " + std::string(varname));
                for (const Token *tok2 = tok; tok2; tok2 = tok2->next())
                {
                    if (tok2->str() == ";")
                        break;

                    if (Token::Match(tok2, pattern.c_str()))
                    {
                        rhs = true;
                        break;
                    }
                }

                addtoken((rhs ? "use" : "assign"));
            }
        }

        if (Token::Match(tok->previous(), "[;{})=] %var%"))
        {
            AllocType dealloc = getDeallocationType(tok, varnames);
            if (dealloc != No)
            {
                addtoken("dealloc");

                if (dealloctype != No && dealloctype != dealloc)
                    dealloc = Many;

                if (dealloc != Many && alloctype != No && alloctype != Many && alloctype != dealloc)
                {
                    callstack.push_back(tok);
                    mismatchAllocDealloc(callstack, varname);
                    callstack.pop_back();
                }
                dealloctype = dealloc;
                continue;
            }
        }

        // if else switch
        if (tok->str() == "if")
        {
            if (Token::simpleMatch(tok, std::string("if ( " + varnameStr + " )").c_str()))
            {
                addtoken("if(var)");

                // Make sure the "use" will not be added
                while (tok->str() != ")")
                    tok = tok->next();
            }
            else if (Token::simpleMatch(tok, std::string("if ( " + varnameStr + " == -1 )").c_str()) ||
                     Token::simpleMatch(tok, std::string("if ( " + varnameStr + " < 0 )").c_str()))
            {
                // FIXME: ensure then this variable has int type and uses as file descriptor
                addtoken("if(!var)");
            }
            else if (Token::simpleMatch(tok, "if (") && notvar(tok->tokAt(2), varnames, true))
            {
                addtoken("if(!var)");
            }
            else
            {
                // Check if the condition depends on var somehow..
                bool dep = false;
                int parlevel = 0;
                for (const Token *tok2 = tok; tok2; tok2 = tok2->next())
                {
                    if (tok2->str() == "(")
                        ++parlevel;
                    if (tok2->str() == ")")
                    {
                        --parlevel;
                        if (parlevel <= 0)
                            break;
                    }
                    if (Token::Match(tok2, std::string("close|fclose|closedir ( " + varnameStr + " )").c_str()))
                    {
                        addtoken("dealloc");
                        addtoken(";");
                        dep = true;
                        break;
                    }
                    if ((tok2->str() != ".") &&
                        Token::simpleMatch(tok2->next(), varnameStr.c_str()) &&
                        !Token::simpleMatch(tok2->next(), std::string(varnameStr + " .").c_str()))
                    {
                        dep = true;
                        break;
                    }
                }

                if (Token::simpleMatch(tok, std::string("if ( ! " + varnameStr + " &&").c_str()))
                {
                    addtoken("if(!var)");
                }
                else if (tok->next() &&
                         tok->next()->link() &&
                         Token::simpleMatch(tok->next()->link()->tokAt(-3), std::string("&& ! " + varnameStr).c_str()))
                {
                    addtoken("if(!var)");
                }
                else
                {
                    addtoken((dep ? "ifv" : "if"));
                }
            }
        }

        if ((tok->str() == "else") || (tok->str() == "switch"))
        {
            addtoken(tok->str().c_str());
        }

        else if ((tok->str() == "case"))
        {
            addtoken("case");
            addtoken(";");
        }

        else if ((tok->str() == "default"))
        {
            addtoken("default");
            addtoken(";");
        }

        // Loops..
        else if ((tok->str() == "for") || (tok->str() == "while"))
        {
            addtoken("loop");
            isloop = true;
        }
        else if ((tok->str() == "do"))
        {
            addtoken("do");
        }
        if (isloop && notvar(tok, varnames))
            addtoken("!var");

        // continue / break..
        if (tok->str() == "continue")
            addtoken("continue");
        if (tok->str() == "break")
            addtoken("break");

        // goto..
        if (tok->str() == "goto")
        {
            addtoken("goto");
        }

        // Return..
        else if (tok->str() == "return")
        {
            addtoken("return");

            // Returning a auto_ptr of this allocated variable..
            if (Token::simpleMatch(tok->next(), "std :: auto_ptr <"))
            {
                const Token *tok2 = tok->tokAt(5);
                while (tok2 && tok2->str() != ">")
                    tok2 = tok2->next();
                if (Token::simpleMatch(tok2, ("> ( " + varnameStr + " )").c_str()))
                {
                    addtoken("use");
                    tok = tok2->tokAt(3);
                }
            }

            else if (Token::Match(tok, ("return &| " + varnameStr).c_str()))
            {
                addtoken("use");
            }

            else if (Token::Match(tok, ("return strcpy|strncpy|memcpy ( " + varnameStr).c_str()))
            {
                addtoken("use");
                tok = tok->tokAt(2);
            }

            else
            {
                for (const Token *tok2 = tok->next(); tok2; tok2 = tok2->next())
                {
                    if (tok2->str() == ";")
                        break;

                    if (tok2->str() == varname)
                    {
                        addtoken("use");
                        break;
                    }
                }
            }
        }

        // throw..
        else if (Token::Match(tok, "try|throw|catch"))
            addtoken(tok->strAt(0));

        // exit..
        if (Token::Match(tok->previous(), "[{};] exit ( %any% ) ;"))
        {
            addtoken("exit");
            tok = tok->tokAt(3);
        }

        // Assignment..
        if (Token::Match(tok, std::string("[)=] " + varnameStr + " [+;)]").c_str()) ||
            Token::Match(tok, std::string(varnameStr + " +=|-=").c_str()) ||
            Token::Match(tok, std::string("+=|<< " + varnameStr + " ;").c_str()) ||
            Token::Match(tok, std::string("= strcpy|strcat|memmove|memcpy ( " + varnameStr + " ,").c_str()))
        {
            addtoken("use");
        }
        else if (Token::Match(tok->previous(), std::string("[;{}=(,+-*/] " + varnameStr + " [").c_str()))
        {
            addtoken("use_");
        }

        // Investigate function calls..
        if (Token::Match(tok, "%var% ("))
        {
            // Inside class function.. if the var is passed as a parameter then
            // just add a "::use"
            // The "::use" means that a member function was probably called but it wasn't analyzed further
            if (classmember)
            {
                int parlevel = 1;
                for (const Token *tok2 = tok->tokAt(2); tok2; tok2 = tok2->next())
                {
                    if (tok2->str() == "(")
                        ++parlevel;
                    else if (tok2->str() == ")")
                    {
                        --parlevel;
                        if (parlevel <= 0)
                            break;
                    }
                    if (tok2->str() == varnameStr)
                    {
                        addtoken("::use");
                        break;
                    }
                }
            }

            else
            {
                const char *str = call_func(tok, callstack, varnames, alloctype, dealloctype, all, sz);
                if (str)
                {
                    if (Token::simpleMatch(tok->tokAt(-2), (varnameStr + " =").c_str()) || std::string(str) != "alloc")
                        addtoken(str);
                }
                else
                {
                    if (getReallocationType(tok) != No &&
                        Token::simpleMatch(tok->tokAt(2), varnameStr.c_str())
                       )
                    {
                        addtoken("if");
                        addtoken("{");
                        addtoken("dealloc");
                        addtoken(";");
                        addtoken("}");
                        tok = tok->tokAt(2);
                        continue;
                    }
                }
            }
        }

        // Callback..
        bool matchFirst = Token::Match(tok, "( %var%");
        if (matchFirst || Token::Match(tok, "( * %var%"))
        {
            int tokIdx = matchFirst ? 2 : 3;

            while (Token::Match(tok->tokAt(tokIdx), ". %var%"))
                tokIdx += 2;

            if (Token::simpleMatch(tok->tokAt(tokIdx), ") ("))
            {
                for (const Token *tok2 = tok->tokAt(tokIdx + 2); tok2; tok2 = tok2->next())
                {
                    if (Token::Match(tok2, "[;{]"))
                        break;
                    else if (tok2->str() == varname)
                    {
                        addtoken("use");
                        break;
                    }
                }
            }
        }

        // Linux lists..
        if (Token::Match(tok, std::string("[=(,] & " + varnameStr + " [.[,)]").c_str()))
        {
            addtoken("&use");
        }
    }

    return rethead;
}






void CheckMemoryLeakInFunction::simplifycode(Token *tok, bool &all)
{
    // Replace "throw" that is not in a try block with "return"
    int indentlevel = 0;
    int trylevel = -1;
    for (Token *tok2 = tok; tok2; tok2 = tok2->next())
    {
        if (tok2->str() == "{")
            ++indentlevel;
        else if (tok2->str() == "}")
        {
            --indentlevel;
            if (indentlevel <= trylevel)
                trylevel = -1;
        }
        else if (trylevel == -1 && tok2->str() == "try")
            trylevel = indentlevel;
        else if (trylevel == -1 && tok2->str() == "throw")
            tok2->str("return");
    }

    // If "--all" is given, remove all "callfunc"..
    if (_settings->_showAll)
    {
        Token *tok2 = tok;
        while (tok2)
        {
            if (tok2->str() == "callfunc")
            {
                tok2->deleteThis();
                all = true;
            }
            else
                tok2 = tok2->next();
        }
    }

    // reduce the code..
    bool done = false;
    while (! done)
    {
        //tok->printOut("simplifycode loop..");
        done = true;

        for (Token *tok2 = tok; tok2; tok2 = tok2 ? tok2->next() : NULL)
        {
            // Delete extra ";"
            while (Token::Match(tok2, "[;{}] ;"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(2));
                done = false;
            }

            // Replace "{ }" with ";"
            if (Token::simpleMatch(tok2->next(), "{ }"))
            {
                tok2->next()->str(";");
                Token::eraseTokens(tok2->next(), tok2->tokAt(3));
                done = false;
            }

            // Delete braces around a single instruction..
            if (Token::Match(tok2->next(), "{ %var% ; }"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(2));
                Token::eraseTokens(tok2->tokAt(2), tok2->tokAt(4));
                done = false;
            }
            if (Token::Match(tok2->next(), "{ %var% %var% ; }"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(2));
                Token::eraseTokens(tok2->tokAt(3), tok2->tokAt(5));
                done = false;
            }

            // Reduce "if if" => "if"
            else if (Token::Match(tok2, "if if|callfunc"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(2));
                done = false;
            }

            else if (tok2->next() && tok2->next()->str() == "if")
            {
                // Delete empty if that is not followed by an else
                if (Token::Match(tok2->next(), "if ; !!else"))
                {
                    Token::eraseTokens(tok2, tok2->tokAt(2));
                    done = false;
                }

                // Delete "if ; else ;"
                else if (Token::simpleMatch(tok2->next(), "if ; else ;"))
                {
                    Token::eraseTokens(tok2, tok2->tokAt(4));
                    done = false;
                }

                // Two "if alloc ;" after one another.. perhaps only one of them can be executed each time
                else if (!_settings->_showAll && Token::Match(tok2, "[;{}] if alloc ; if alloc ;"))
                {
                    Token::eraseTokens(tok2, tok2->tokAt(4));
                    done = false;
                }

                else if (Token::Match(tok2, "; if ; else assign|use ; assign|use") ||
                         Token::Match(tok2, "; if assign|use ; else ; assign|use"))
                {
                    Token::eraseTokens(tok2, tok2->tokAt(4));
                    done = false;
                }


                // Reduce "if assign|dealloc|use ;" that is not followed by an else..
                // If "--all" has been given these are deleted
                // Otherwise, only the "if" will be deleted
                else if (Token::Match(tok2, "[;{}] if assign|dealloc|use ; !!else"))
                {
                    if (_settings->_showAll)
                    {
                        Token::eraseTokens(tok2, tok2->tokAt(3));
                        all = true;
                    }
                    else
                    {
                        Token::eraseTokens(tok2, tok2->tokAt(2));
                    }
                    done = false;
                }

                // Reduce "if return ; alloc ;" => "alloc ;"
                else if (Token::Match(tok2, "[;{}] if return ; alloc ;"))
                {
                    Token::eraseTokens(tok2, tok2->tokAt(4));
                    done = false;
                }

                // "[;{}] if alloc ; else return ;" => "[;{}] alloc ;"
                else if (Token::Match(tok2, "[;{}] if alloc ; else return ;"))
                {
                    Token::eraseTokens(tok2, tok2->tokAt(2));        // Remove "if"
                    Token::eraseTokens(tok2->next(), tok2->tokAt(5));  // Remove "; else return"
                    done = false;
                }

                // Reduce "if ; else %var% ;" => "if %var% ;"
                else if (Token::Match(tok2->next(), "if ; else %var% ;"))
                {
                    Token::eraseTokens(tok2->next(), tok2->tokAt(4));
                    done = false;
                }

                // Reduce "if ; else return use ;" => "if return use ;"
                else if (Token::simpleMatch(tok2->next(), "if ; else return use ;"))
                {
                    Token::eraseTokens(tok2->next(), tok2->tokAt(4));
                    done = false;
                }

                // Reduce "if return ; if return ;" => "if return ;"
                else if (Token::simpleMatch(tok2->next(), "if return ; if return ;"))
                {
                    Token::eraseTokens(tok2, tok2->tokAt(4));
                    done = false;
                }

                // Delete first if in .. "if { dealloc|assign|use ; return ; } if return ;"
                else if (Token::Match(tok2, "[;{}] if { dealloc|assign|use ; return ; } if return ;"))
                {
                    Token::eraseTokens(tok2, tok2->tokAt(8));
                    done = false;
                }

                // Remove "if { dealloc ; callfunc ; } !!else"
                else if (Token::Match(tok2->next(), "if { dealloc|assign ; callfunc ; } !!else"))
                {
                    Token::eraseTokens(tok2, tok2->tokAt(8));
                    done = false;
                }

                // Reducing if..
                else if (_settings->_showAll)
                {
                    if (Token::Match(tok2, "[;{}] if { assign|dealloc|use ; return ; } !!else"))
                    {
                        all = true;
                        Token::eraseTokens(tok2, tok2->tokAt(8));
                        done = false;
                    }
                }

                continue;
            }

            // Reduce "if(var) dealloc ;" and "if(var) use ;" that is not followed by an else..
            if (Token::Match(tok2, "[;{}] if(var) assign|dealloc|use ; !!else"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(2));
                done = false;
            }

            // Reduce "; if(!var) alloc ; !!else" => "; dealloc ; alloc ;"
            if (Token::Match(tok2, "; if(!var) alloc ; !!else"))
            {
                // Remove the "if(!var)"
                Token::eraseTokens(tok2, tok2->tokAt(2));

                // Insert "dealloc ;" before the "alloc ;"
                tok2->insertToken(";");
                tok2->insertToken("dealloc");

                done = false;
            }

            // Remove "catch ;"
            if (Token::simpleMatch(tok2->next(), "catch ;"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(3));
                done = false;
            }

            // Reduce "if* ;" that is not followed by an else..
            if (Token::Match(tok2->next(), "if(var)|if(!var)|ifv ; !!else"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(2));
                done = false;
            }

            // Reduce "else ;" => ";"
            if (Token::simpleMatch(tok2->next(), "else ;"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(2));
                done = false;
            }

            // Delete if block: "alloc; if return use ;"
            if (Token::Match(tok2, "alloc ; if return use ; !!else"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(5));
                done = false;
            }

            // Reduce "alloc|dealloc|use ; exit ;" => "; exit ;"
            if (Token::Match(tok2, "alloc|dealloc|use|callfunc ; exit ;"))
            {
                tok2->deleteThis();
                done = false;
            }

            // Reduce "alloc|dealloc|use ; if(var) exit ;"
            if (Token::Match(tok2, "alloc|dealloc|use ; if(var) exit ;"))
            {
                tok2->deleteThis();
                done = false;
            }

            // Remove "if exit ;"
            if (Token::simpleMatch(tok2, "if exit ;"))
            {
                tok2->deleteThis();
                tok2->deleteThis();
                done = false;
            }

            // Replace "dealloc use ;" with "dealloc ;"
            if (Token::simpleMatch(tok2, "dealloc use ;"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(2));
                done = false;
            }

            // Remove the "if break|continue ;" that follows "dealloc ; alloc ;"
            if (! _settings->_showAll && Token::Match(tok2, "dealloc ; alloc ; if break|continue ;"))
            {
                tok2 = tok2->tokAt(3);
                Token::eraseTokens(tok2, tok2->tokAt(3));
                done = false;
            }

            // Reduce "do { alloc ; } " => "alloc ;"
            /** @todo If the loop "do { alloc ; }" can be executed twice, reduce it to "loop alloc ;" */
            if (Token::simpleMatch(tok2->next(), "do { alloc ; }"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(3));
                Token::eraseTokens(tok2->tokAt(2), tok2->tokAt(4));
                done = false;
            }

            // Reduce "loop if break ; => ";"
            if (Token::Match(tok2->next(), "loop if break|continue ; !!else"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(4));
                done = false;
            }

            // Reduce "loop { assign|dealloc|use ; alloc ; if break ; }" to "assign|dealloc|use ; alloc ;"
            if (Token::Match(tok2->next(), "loop { assign|dealloc|use ; alloc ; if break|continue ; }"))
            {
                // erase "loop {"
                Token::eraseTokens(tok2, tok2->tokAt(3));
                // erase "if break|continue ; }"
                tok2 = tok2->tokAt(4);
                Token::eraseTokens(tok2, tok2->tokAt(5));
                done = false;
            }

            // Replace "loop { X ; break ; }" with "X ;"
            if (Token::Match(tok2->next(), "loop { %var% ; break ; }"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(3));
                Token::eraseTokens(tok2->tokAt(2), tok2->tokAt(6));
                done = false;
            }

            // Replace "loop ;" with ";"
            if (Token::simpleMatch(tok2->next(), "loop ;"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(2));
                done = false;
            }

            // Replace "loop !var ;" with ";"
            if (Token::Match(tok2->next(), "loop !var ;"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(4));
                done = false;
            }

            // Replace "loop if return ;" with "if return ;"
            if (Token::simpleMatch(tok2->next(), "loop if return"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(2));
                done = false;
            }

            // Delete if block in "alloc ; if(!var) return ;"
            if (Token::Match(tok2, "alloc ; if(!var) return ;"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(4));
                done = false;
            }

            // Delete if block: "alloc; if return use ;"
            if (Token::Match(tok2, "alloc ; if return use ; !!else"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(5));
                done = false;
            }

            // Reduce "[;{}] return ; %var%" => "[;{}] return ;"
            if (Token::Match(tok2, "[;{}] return ; %var%"))
            {
                Token::eraseTokens(tok2->tokAt(2), tok2->tokAt(4));
                done = false;
            }

            // Reduce "[;{}] return use ; %var%" => "[;{}] return use ;"
            if (Token::Match(tok2, "[;{}] return use ; %var%"))
            {
                Token::eraseTokens(tok2->tokAt(3), tok2->tokAt(5));
                done = false;
            }

            // Reduce "if(var) return use ;" => "return use ;"
            if (Token::Match(tok2->next(), "if(var) return use ; !!else"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(2));
                done = false;
            }

            // Reduce "if(var) assign|dealloc|use ;" => "assign|dealloc|use ;"
            if (Token::Match(tok2->next(), "if(var) assign|dealloc|use ; !!else"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(2));
                done = false;
            }

            // malloc - realloc => alloc ; dealloc ; alloc ;
            // Reduce "[;{}] alloc ; dealloc ; alloc ;" => "[;{}] alloc ;"
            if (Token::Match(tok2, "[;{}] alloc ; dealloc ; alloc ;"))
            {
                Token::eraseTokens(tok2->next(), tok2->tokAt(6));
                done = false;
            }

            // Delete second use in "use ; use ;"
            while (Token::Match(tok2, "[;{}] use ; use ;"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(3));
                done = false;
            }

            // Delete first part in "use ; dealloc ;"
            if (Token::Match(tok2, "[;{}] use ; dealloc ;"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(3));
                done = false;
            }

            // Delete first part in "use ; return use ;"
            if (Token::Match(tok2, "[;{}] use ; return use ;"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(2));
                done = false;
            }

            // Delete "callfunc ;" that is followed by "use|if|callfunc"
            // If the function doesn't throw exception or exit the application, then the "callfunc" is not needed
            if (Token::Match(tok2, "callfunc ; use|if|callfunc"))
            {
                tok2->deleteThis();
                done = false;
            }

            // Delete second case in "case ; case ;"
            while (Token::simpleMatch(tok2, "case ; case ;"))
            {
                Token::eraseTokens(tok2, tok2->tokAt(3));
                done = false;
            }

            // Replace switch with if (if not complicated)
            if (Token::simpleMatch(tok2, "switch {"))
            {
                // Right now, I just handle if there are a few case and perhaps a default.
                bool valid = false;
                bool incase = false;
                for (const Token * _tok = tok2->tokAt(2); _tok; _tok = _tok->next())
                {
                    if (_tok->str() == "{")
                        break;

                    else if (_tok->str() == "}")
                    {
                        valid = true;
                        break;
                    }

                    else if (strncmp(_tok->str().c_str(), "if", 2) == 0)
                        break;

                    else if (_tok->str() == "switch")
                        break;

                    else if (_tok->str() == "loop")
                        break;

                    else if (incase && _tok->str() == "case")
                        break;

                    incase |= (_tok->str() == "case");
                    incase &= (_tok->str() != "break");
                }

                if (!incase && valid)
                {
                    done = false;
                    tok2->str(";");
                    Token::eraseTokens(tok2, tok2->tokAt(2));
                    tok2 = tok2->next();
                    bool first = true;
                    while (Token::Match(tok2, "case|default"))
                    {
                        bool def(tok2->str() == "default");
                        tok2->str(first ? "if" : "}");
                        if (first)
                        {
                            first = false;
                            tok2->insertToken("{");
                        }
                        else
                        {
                            // Insert "else [if] {
                            tok2->insertToken("{");
                            if (! def)
                                tok2->insertToken("if");
                            tok2->insertToken("else");
                            tok2 = tok2->next();
                        }
                        while (tok2 && tok2->str() != "}" && ! Token::simpleMatch(tok2, "break ;"))
                            tok2 = tok2->next();
                        if (Token::simpleMatch(tok2, "break ;"))
                        {
                            tok2->str(";");
                            tok2 = tok2->tokAt(2);
                        }
                    }
                }
            }
        }
    }


}









// Check for memory leaks for a function variable.
void CheckMemoryLeakInFunction::checkScope(const Token *Tok1, const char varname[], bool classmember, unsigned int sz)
{
    std::list<const Token *> callstack;

    AllocType alloctype = No;
    AllocType dealloctype = No;

    bool all = false;

    const Token *result;

    Token *tok = getcode(Tok1, callstack, varname, alloctype, dealloctype, classmember, all, sz);
    //tok->printOut((std::string("Checkmemoryleak: getcode result for: ") + varname).c_str());

    // Simplify the code and check if freed memory is used..
    for (Token *tok2 = tok; tok2; tok2 = tok2->next())
    {
        while (Token::Match(tok2, "[;{}] ;"))
            Token::eraseTokens(tok2, tok2->tokAt(2));
    }
    if ((result = Token::findmatch(tok, "[;{}] dealloc ; use_ ;")) != NULL)
    {
        deallocuseError(result->tokAt(3), varname);
    }

    // Replace "&use" with "use". Replace "use_" with ";"
    for (Token *tok2 = tok; tok2; tok2 = tok2->next())
    {
        if (tok2->str() == "&use")
            tok2->str("use");
        else if (tok2->str() == "use_")
            tok2->str(";");
        else if (tok2->str() == "::use")    // Some kind of member function usage. Not analyzed very well.
            tok2->str("use");
        else if (tok2->str() == "recursive")
            tok2->str("use");
        else if (tok2->str() == "dealloc_")
            tok2->str("dealloc");
        else if (tok2->str() == "realloc")
        {
            tok2->str("dealloc");
            tok2->insertToken("alloc");
            tok2->insertToken(";");
        }
    }

    simplifycode(tok, all);

    if (_settings->_debug)
    {
        tok->printOut((std::string("Checkmemoryleak: simplifycode result for: ") + varname).c_str());
    }

    // If the variable is not allocated at all => no memory leak
    if (Token::findmatch(tok, "alloc") == 0)
    {
        Tokenizer::deleteTokens(tok);
        return;
    }

    /** @todo handle "goto" */
    if (Token::findmatch(tok, "goto"))
    {
        Tokenizer::deleteTokens(tok);
        return;
    }

    if ((result = Token::findmatch(tok, "loop alloc ;")) != NULL)
    {
        memoryLeak(result, varname, alloctype, all);
    }

    else if ((result = Token::findmatch(tok, "alloc ; if break|continue|return ;")) != NULL
             && Token::findmatch(tok, "dealloc ; alloc ; if continue ;") == NULL)
    {
        memoryLeak(result->tokAt(3), varname, alloctype, all);
    }

    else if (_settings->_showAll && (result = Token::findmatch(tok, "alloc ; ifv break|continue|return ;")) != NULL)
    {
        memoryLeak(result->tokAt(3), varname, alloctype, all);
    }

    else if ((result = Token::findmatch(tok, "alloc ; alloc|assign|return callfunc| ;")) != NULL)
    {
        memoryLeak(result->tokAt(2), varname, alloctype, all);
    }

    else if ((result = Token::findmatch(tok, "dealloc ; dealloc ;")) != NULL)
    {
        deallocDeallocError(result->tokAt(2), varname);
    }

    else if (! Token::findmatch(tok, "dealloc") &&
             ! Token::findmatch(tok, "use") &&
             ! Token::findmatch(tok, "return use ;"))
    {
        const Token *last = tok;
        while (last->next())
            last = last->next();
        memoryLeak(last, varname, alloctype, all);
    }

    else if ((result = Token::findmatch(tok, "alloc ; }")) != NULL)
    {
        if (result->tokAt(3) == NULL)
            memoryLeak(result->tokAt(2), varname, alloctype, all);
    }

    // detect cases that "simplifycode" don't handle well..
    else if (_settings->_debug)
    {
        Token *first = tok;
        while (first && first->str() == ";")
            first = first->next();

        bool noerr = false;
        noerr |= Token::simpleMatch(first, "alloc ; }");
        noerr |= Token::simpleMatch(first, "alloc ; dealloc ; }");
        noerr |= Token::simpleMatch(first, "alloc ; return use ; }");
        noerr |= Token::simpleMatch(first, "alloc ; use ; }");
        noerr |= Token::simpleMatch(first, "alloc ; use ; return ; }");
        noerr |= Token::simpleMatch(first, "if alloc ; dealloc ; }");
        noerr |= Token::simpleMatch(first, "if alloc ; return use ; }");
        noerr |= Token::simpleMatch(first, "if alloc ; use ; }");
        noerr |= Token::simpleMatch(first, "alloc ; ifv return ; dealloc ; }");
        noerr |= Token::simpleMatch(first, "alloc ; if return ; dealloc; }");

        // Unhandled case..
        if (! noerr)
        {
            std::cout << "Token listing..\n  ";
            for (const Token *tok2 = tok; tok2; tok2 = tok2->next())
                std::cout << " " << tok2->str();
            std::cout << "\n";
        }
    }

    Tokenizer::deleteTokens(tok);
}
//---------------------------------------------------------------------------









//---------------------------------------------------------------------------
// Checks for memory leaks inside function..
//---------------------------------------------------------------------------

void CheckMemoryLeakInFunction::check()
{
    bool classmember = false;
    bool beforeParameters = false;
    bool infunc = false;
    int indentlevel = 0;
    for (const Token *tok = _tokenizer->tokens(); tok; tok = tok->next())
    {
        if (tok->str() == "{")
            ++indentlevel;

        else if (tok->str() == "}")
            --indentlevel;

        // Skip these weird blocks... "( { ... } )"
        else if (Token::simpleMatch(tok, "( {"))
        {
            tok = tok->link();
            if (!tok)
                break;
            continue;
        }

        // In function..
        if (indentlevel == 0)
        {
            if (Token::simpleMatch(tok, ") {"))
                infunc = true;

            else if (tok->str() == "(")
                beforeParameters = false;

            else if (tok->str() == "::" && beforeParameters)
                classmember = true;

            else if (Token::Match(tok, "[;}]"))
            {
                infunc = classmember = false;
                beforeParameters = true;
            }
        }

        // Declare a local variable => Check
        if (indentlevel > 0 && infunc)
        {
            unsigned int sz = _tokenizer->sizeOfType(tok->strAt(1));
            if (sz < 1)
                sz = 1;

            if (!Token::Match(tok, "[{};] %type%"))
                continue;

            // Don't check static variables
            if (tok->next()->str() == "static")
                continue;

            if (Token::Match(tok, "[{};] %type% * const| %var% [;=]"))
            {
                const int varname_tok = (tok->tokAt(3)->str() != "const" ? 3 : 4);
                checkScope(tok->next(), tok->strAt(varname_tok), classmember, sz);
            }

            else if (Token::Match(tok, "[{};] %type% %type% * const| %var% [;=]"))
            {
                const int varname_tok = (tok->tokAt(4)->str() != "const" ? 4 : 5);
                checkScope(tok->next(), tok->strAt(varname_tok), classmember, sz);
            }

            else if (Token::Match(tok, "[{};] int %var% [;=]"))
            {
                checkScope(tok->next(), tok->strAt(2), classmember, sz);
            }
        }
    }
}
//---------------------------------------------------------------------------





























//---------------------------------------------------------------------------
// Checks for memory leaks in classes..
//---------------------------------------------------------------------------



void CheckMemoryLeakInClass::check()
{
    int indentlevel = 0;
    for (const Token *tok = _tokenizer->tokens(); tok; tok = tok->next())
    {
        if (tok->str() == "{")
            ++indentlevel;

        else if (tok->str() == "}")
            --indentlevel;

        else if (indentlevel == 0 && Token::Match(tok, "class %var% [{:]"))
        {
            std::vector<const char *> classname;
            classname.push_back(tok->strAt(1));
            parseClass(tok, classname);
        }
    }
}


void CheckMemoryLeakInClass::parseClass(const Token *tok1, std::vector<const char *> &classname)
{
    // Go into class.
    while (tok1 && tok1->str() != "{")
        tok1 = tok1->next();
    if (tok1)
        tok1 = tok1->next();

    int indentlevel = 0;
    for (const Token *tok = tok1; tok; tok = tok->next())
    {
        if (tok->str() == "{")
            ++indentlevel;

        else if (tok->str() == "}")
        {
            --indentlevel;
            if (indentlevel < 0)
                return;
        }

        // Only parse this particular class.. not subclasses
        if (indentlevel > 0)
            continue;

        // Declaring subclass.. recursive checking
        if (Token::Match(tok, "class %var% [{:]"))
        {
            classname.push_back(tok->strAt(1));
            parseClass(tok, classname);
            classname.pop_back();
        }

        // Declaring member variable.. check allocations and deallocations
        if (Token::Match(tok->next(), "%type% * %var% ;"))
        {
            // No false positives for auto deallocated classes..
            if (_settings->isAutoDealloc(tok->strAt(1)))
                continue;

            if (tok->isName() || Token::Match(tok, "[;}]"))
            {
                if (_settings->_showAll || !isclass(_tokenizer, tok->tokAt(1)))
                    variable(classname.back(), tok->tokAt(3));
            }
        }
    }
}

void CheckMemoryLeakInClass::variable(const char classname[], const Token *tokVarname)
{
    const char *varname = tokVarname->strAt(0);

    // Check if member variable has been allocated and deallocated..
    CheckMemoryLeak::AllocType Alloc = CheckMemoryLeak::No;
    CheckMemoryLeak::AllocType Dealloc = CheckMemoryLeak::No;

    // Loop through all tokens. Inspect member functions
    int indent_ = 0;
    const Token *functionToken = Tokenizer::findClassFunction(_tokenizer->tokens(), classname, "~| %var%", indent_);
    while (functionToken)
    {
        int indent = 0;
        bool initlist = false;
        for (const Token *tok = functionToken; tok; tok = tok->next())
        {
            if (tok->str() == "{")
                ++indent;
            else if (tok->str() == "}")
            {
                --indent;
                if (indent <= 0)
                    break;
            }
            else if (indent == 0 && Token::simpleMatch(tok, ") :"))
                initlist = true;
            else if (initlist || indent > 0)
            {
                if (indent == 0)
                {
                    if (!Token::Match(tok, (":|, " + std::string(varname) + " (").c_str()))
                        continue;
                }

                // Allocate..
                if (indent == 0 || Token::Match(tok, (std::string(varname) + " =").c_str()))
                {
                    AllocType alloc = getAllocationType(tok->tokAt((indent > 0) ? 2 : 3));
                    if (alloc != CheckMemoryLeak::No)
                    {
                        if (Alloc != No && Alloc != alloc)
                            alloc = CheckMemoryLeak::Many;

                        std::list<const Token *> callstack;
                        if (alloc != CheckMemoryLeak::Many && Dealloc != CheckMemoryLeak::No && Dealloc != CheckMemoryLeak::Many && Dealloc != alloc)
                        {
                            callstack.push_back(tok);
                            mismatchAllocDealloc(callstack, (std::string(classname) + "::" + varname).c_str());
                            callstack.pop_back();
                        }

                        Alloc = alloc;
                    }
                }

                if (indent == 0)
                    continue;

                // Deallocate..
                const char *varnames[3] = { "var", 0, 0 };
                varnames[0] = varname;
                AllocType dealloc = getDeallocationType(tok, varnames);
                if (dealloc == No)
                {
                    varnames[0] = "this";
                    varnames[1] = varname;
                    dealloc = getDeallocationType(tok, varnames);
                }
                if (dealloc != CheckMemoryLeak::No)
                {
                    if (Dealloc != CheckMemoryLeak::No && Dealloc != dealloc)
                        dealloc = CheckMemoryLeak::Many;

                    std::list<const Token *> callstack;
                    if (dealloc != CheckMemoryLeak::Many && Alloc != CheckMemoryLeak::No &&  Alloc != Many && Alloc != dealloc)
                    {
                        callstack.push_back(tok);
                        mismatchAllocDealloc(callstack, (std::string(classname) + "::" + varname).c_str());
                        callstack.pop_back();
                    }

                    Dealloc = dealloc;
                }

            }
        }

        functionToken = Tokenizer::findClassFunction(functionToken->next(), classname, "~| %var%", indent_);
    }

    if (_settings->_showAll && Alloc != CheckMemoryLeak::No && Dealloc == CheckMemoryLeak::No)
    {
        memoryLeak(tokVarname, (std::string(classname) + "::" + varname).c_str(), Alloc, true);
    }
}








void CheckMemoryLeakStructMember::check()
{
    // This should be in the CheckMemoryLeak base class
    std::set<std::string> ignoredFunctions;
    ignoredFunctions.insert("if");
    ignoredFunctions.insert("for");
    ignoredFunctions.insert("while");
    ignoredFunctions.insert("malloc");

    for (const Token *tok = _tokenizer->tokens(); tok; tok = tok->next())
    {
        // Locate struct variables..
        if (Token::Match(tok, "struct|;|{|} %type% * %var% [=;]"))
        {
            const Token * const vartok = tok->tokAt(3);
            if (vartok->varId() == 0)
                continue;

            // Check that struct is allocated..
            {
                const unsigned int varid(vartok->varId());
                bool alloc = false;
                unsigned int indentlevel2 = 0;
                for (const Token *tok2 = vartok; tok2; tok2 = tok2->next())
                {
                    if (tok2->str() == "{")
                        ++indentlevel2;
                    else if (tok2->str() == "}")
                    {
                        if (indentlevel2 == 0)
                            break;
                        --indentlevel2;
                    }
                    else if (Token::Match(tok2, "= %varid% [;=]", varid))
                    {
                        alloc = false;
                        break;
                    }
                    else if (Token::Match(tok2, "%varid% = malloc|kmalloc (", varid))
                    {
                        alloc = true;
                    }
                }
                if (!alloc)
                    continue;
            }

            // Check struct..
            unsigned int indentlevel2 = 0;
            for (const Token *tok2 = vartok; tok2; tok2 = tok2->next())
            {
                if (tok2->str() == "{")
                    ++indentlevel2;

                else if (tok2->str() == "}")
                {
                    if (indentlevel2 == 0)
                        break;
                    --indentlevel2;
                }

                // Unknown usage of struct
                /** @todo Check how the struct is used. Only bail out if necessary */
                else if (Token::Match(tok2, "[(,] %varid% [,)]", vartok->varId()))
                    break;

                // Struct member is allocated => check if it is also properly deallocated..
                else if (Token::Match(tok2, "%varid% . %var% = malloc|strdup|kmalloc (", vartok->varId()))
                {
                    const unsigned int structid(vartok->varId());
                    const unsigned int structmemberid(tok2->tokAt(2)->varId());

                    // This struct member is allocated.. check that it is deallocated
                    unsigned int indentlevel3 = indentlevel2;
                    for (const Token *tok3 = tok2; tok3; tok3 = tok3->next())
                    {
                        if (tok3->str() == "{")
                            ++indentlevel3;

                        else if (tok3->str() == "}")
                        {
                            if (indentlevel3 == 0)
                            {
                                memoryLeak(tok3, (vartok->str() + "." + tok2->strAt(2)).c_str(), Malloc, false);
                                break;
                            }
                            --indentlevel3;
                        }

                        // Deallocating the struct member..
                        else if (Token::Match(tok3, "free|kfree ( %var% . %varid% )", structmemberid))
                        {
                            // If the deallocation happens at the base level, don't check this member anymore
                            if (indentlevel3 == 0)
                                break;

                            // deallocating and then returning from function in a conditional block =>
                            // skip ahead out of the block
                            bool ret = false;
                            while (tok3)
                            {
                                // debug info
                                const std::string tok3str_(tok3->str());
                                if (tok3->str() == "return")
                                    ret = true;
                                else if (tok3->str() == "{" || tok3->str() == "}")
                                    break;
                                tok3 = tok3->next();
                            }
                            if (!ret || !tok3 || tok3->str() != "}")
                                break;
                            --indentlevel3;
                            continue;
                        }

                        // Deallocating the struct..
                        else if (indentlevel2 == 0 && Token::Match(tok3, "free|kfree ( %varid% )", structid))
                        {
                            memoryLeak(tok3, (vartok->str() + "." + tok2->strAt(2)).c_str(), Malloc, false);
                            break;
                        }

                        // failed allocation => skip code..
                        else if (Token::Match(tok3, "if ( ! %var% . %varid% )", structmemberid))
                        {
                            // Goto the ")"
                            while (tok3->str() != ")")
                                tok3 = tok3->next();

                            // Skip block..
                            unsigned int indentlevel = 0;
                            while (tok3)
                            {
                                if (tok3->str() == "{")
                                    ++indentlevel;

                                else if (tok3->str() == "}")
                                {
                                    if (indentlevel <= 1)
                                        break;
                                    --indentlevel;
                                }
                                tok3 = tok3->next();
                            }
                        }

                        // Returning from function..
                        else if (tok3->str() == "return")
                        {
                            // Returning from function without deallocating struct member?
                            if (!Token::Match(tok3, "return %varid% ;", structid))
                            {
                                memoryLeak(tok3, (vartok->str() + "." + tok2->strAt(2)).c_str(), Malloc, false);
                            }
                            break;
                        }

                        // goto isn't handled well.. bail out even though there might be leaks
                        else if (tok3->str() == "goto")
                            break;

                        // using struct in a function call..
                        else if (Token::Match(tok3, "%var% ("))
                        {
                            // Calling non-function / function that doesn't deallocate?
                            if (ignoredFunctions.find(tok3->str()) != ignoredFunctions.end())
                                continue;

                            // Check if the struct is used..
                            bool deallocated = false;
                            unsigned int parlevel = 0;
                            for (const Token *tok4 = tok3; tok4; tok4 = tok4->next())
                            {
                                if (tok4->str() == "(")
                                    ++parlevel;

                                else if (tok4->str() == ")")
                                {
                                    if (parlevel <= 1)
                                        break;
                                    --parlevel;
                                }

                                if (Token::Match(tok4, "[(,] %varid% [,)]", structid))
                                {
                                    /** @todo check if the function deallocates the memory */
                                    deallocated = true;
                                    break;
                                }
                            }

                            if (deallocated)
                                break;
                        }
                    }
                }
            }
        }
    }
}



