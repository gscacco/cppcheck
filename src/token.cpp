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

#include "token.h"
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <cctype>
#include <sstream>
#include <map>

Token::Token() :
        _str(""),
        _isName(false),
        _isNumber(false),
        _isBoolean(false),
        _varId(0),
        _next(0),
        _previous(0),
        _link(0),
        _fileIndex(0),
        _linenr(0)
{
}

Token::~Token()
{

}

void Token::str(const std::string &s)
{
    _str = s;
    _isName = bool(_str[0] == '_' || std::isalpha(_str[0]));
    _isNumber = bool(std::isdigit(_str[(_str[0] == '-') ? 1 : 0]) != 0);
    if (_str == "true" || _str == "false")
        _isBoolean = true;
    else
        _isBoolean = false;

    _varId = 0;
}

void Token::str(const char s[])
{
    str(std::string(s));
}

void Token::concatStr(std::string const& b)
{
    _str.erase(_str.length() - 1);
    _str.append(b.begin() + 1, b.end());
}

void Token::deleteNext()
{
    Token *n = _next;
    _next = n->next();
    delete n;
    if (_next)
        _next->previous(this);
}

void Token::deleteThis()
{
    if (_next)
    {
        _str = _next->_str;
        _isName = _next->_isName;
        _isNumber = _next->_isNumber;
        _isBoolean = _next->_isBoolean;
        _varId = _next->_varId;
        _fileIndex = _next->_fileIndex;
        _linenr = _next->_linenr;
        _link = _next->_link;
        deleteNext();
    }
    else if (_previous)
    {
        // This should never be used for tokens
        // at the end of the list
        str(";");
    }
    else
    {
        // We are the last token in the list, we can't delete
        // ourselves, so just make us ;
        str(";");
    }
}

void Token::replace(Token *replaceThis, Token *start, Token *end)
{
    // Fix the whole in the old location of start and end
    if (start->previous())
        start->previous()->next(end->next());

    if (end->next())
        end->next()->previous(start->previous());

    // Move start and end to their new location
    if (replaceThis->previous())
        replaceThis->previous()->next(start);

    if (replaceThis->next())
        replaceThis->next()->previous(end);

    start->previous(replaceThis->previous());
    end->next(replaceThis->next());

    // Delete old token, which is replaced
    delete replaceThis;
}

const Token *Token::tokAt(int index) const
{
    const Token *tok = this;
    int num = abs(index);
    while (num > 0 && tok)
    {
        if (index > 0)
            tok = tok->next();
        else
            tok = tok->previous();
        --num;
    }
    return tok;
}

Token *Token::tokAt(int index)
{
    Token *tok = this;
    int num = abs(index);
    while (num > 0 && tok)
    {
        if (index > 0)
            tok = tok->next();
        else
            tok = tok->previous();
        --num;
    }
    return tok;
}

const char *Token::strAt(int index) const
{
    const Token *tok = this->tokAt(index);
    return tok ? tok->_str.c_str() : "";
}

int Token::multiCompare(const char *haystack, const char *needle)
{
    bool emptyStringFound = false;
    bool noMatch = false;
    const char *needlePointer = needle;
    for (; *haystack; ++haystack)
    {
        if (*haystack == '|')
        {
            if (noMatch)
            {
                // We didn't have a match at this round
                noMatch = false;
            }
            else if (*needlePointer == 0)
            {
                // If needle and haystack are both at the end, we have a match.
                return 1;
            }
            else if (needlePointer == needle)
            {
                // If needlePointer was not increased at all, we had a empty
                // string in the haystack
                emptyStringFound = true;
            }

            needlePointer = needle;
            continue;
        }

        if (noMatch)
            continue;

        // If haystack and needle don't share the same character,
        // find next '|' character.
        if (*needlePointer != *haystack)
        {
            noMatch = true;
            continue;
        }

        // All characters in haystack and needle have matched this far
        ++needlePointer;
    }


    if (!noMatch)
    {
        if (*needlePointer == 0)
        {
            // If both needle and haystack are at the end, then we have a match.
            return 1;
        }
        else if (needlePointer == needle)
        {
            // Last string in haystack was empty string e.g. "one|two|"
            return 0;
        }
    }

    // If empty string was found earlier from the haystack
    if (emptyStringFound)
        return 0;

    return -1;
}

bool Token::simpleMatch(const Token *tok, const char pattern[])
{
    const char *current, *next;

    current = pattern;
    next = strchr(pattern, ' ');
    if (!next)
        next = pattern + strlen(pattern);

    while (*current)
    {
        size_t length = static_cast<size_t>(next - current);

        if (!tok || length != tok->_str.length() || strncmp(current, tok->_str.c_str(), length))
            return false;

        current = next;
        if (*next)
        {
            next = strchr(++current, ' ');
            if (!next)
                next = current + strlen(current);
        }
        tok = tok->next();
    }

    return true;
}

bool Token::Match(const Token *tok, const char pattern[], unsigned int varid)
{
    const char *p = pattern;
    bool firstpattern = true;
    while (*p)
    {
        // Skip spaces in pattern..
        while (*p == ' ')
            ++p;

        // Extract token from pattern..
        char str[500];
        char *s = str;
        while (*p && *p != ' ')
        {
            *s = *p;
            ++s;
            ++p;
        }
        *s = 0;

        // No token => Success!
        if (str[0] == 0)
            return true;

        if (!tok)
        {
            // If we have no tokens, pattern "!!else" should return true
            if (str[1] == '!' && str[0] == '!' && str[2] != '\0')
                continue;
            else
                return false;
        }

        // If we are in the first token, we skip all initial !! patterns
        if (firstpattern && !tok->previous() && tok->next() && str[1] == '!' && str[0] == '!' && str[2] != '\0')
            continue;

        firstpattern = false;

        // Compare the first character of the string for optimization reasons
        // before doing more detailed checks.
        bool patternIdentified = false;
        if (str[0] == '%')
        {
            // Any symbolname..
            if (strcmp(str, "%var%") == 0)
            {
                if (!tok->isName())
                    return false;

                patternIdentified = true;
            }

            // Type..
            if (strcmp(str, "%type%") == 0)
            {
                if (!tok->isName())
                    return false;

                if (tok->str() == "delete")
                    return false;

                patternIdentified = true;
            }

            // Accept any token
            else if (strcmp(str, "%any%") == 0)
            {
                patternIdentified = true;
            }

            else if (strcmp(str, "%varid%") == 0)
            {
                if (varid == 0)
                {
                    std::cout << "\n###### If you see this, there is a bug ###### Token::Match() - varid was 0" << std::endl;
                }

                if (tok->varId() != varid)
                    return false;

                patternIdentified = true;
            }

            else if (strcmp(str, "%num%") == 0)
            {
                if (!tok->isNumber())
                    return false;

                patternIdentified = true;
            }

            else if (strcmp(str, "%bool%") == 0)
            {
                if (!tok->isBoolean())
                    return false;

                patternIdentified = true;
            }

            else if (strcmp(str, "%str%") == 0)
            {
                if (tok->_str[0] != '\"')
                    return false;

                patternIdentified = true;
            }
        }

        if (patternIdentified)
        {
            // Pattern was identified already above.
        }

        // [.. => search for a one-character token..
        else if (str[0] == '[' && strchr(str, ']') && tok->_str[1] == 0)
        {
            *strrchr(str, ']') = 0;
            if (strchr(str + 1, tok->_str[0]) == 0)
                return false;
        }

        // Parse multi options, such as void|int|char (accept token which is one of these 3)
        else if (strchr(str, '|') && (str[0] != '|' || strlen(str) > 2))
        {
            int res = multiCompare(str, tok->_str.c_str());
            if (res == 0)
            {
                // Empty alternative matches, use the same token on next round
                continue;
            }
            else if (res == -1)
            {
                // No match
                return false;
            }
        }

        // Parse "not" options. Token can be anything except the given one
        else if (str[1] == '!' && str[0] == '!' && str[2] != '\0')
        {
            if (strcmp(tok->str().c_str(), &(str[2])) == 0)
                return false;
        }

        else if (str != tok->_str)
            return false;

        tok = tok->next();
    }

    // The end of the pattern has been reached and nothing wrong has been found
    return true;
}

bool Token::isName() const
{
    return _isName;
}

bool Token::isNumber() const
{
    return _isNumber;
}

bool Token::isBoolean() const
{
    return _isBoolean;
}

bool Token::isStandardType() const
{
    bool ret = false;
    const char *type[] = {"bool", "char", "short", "int", "long", "float", "double", 0};
    for (int i = 0; type[i]; i++)
        ret |= (_str == type[i]);
    return ret;
}

//---------------------------------------------------------------------------

const Token *Token::findmatch(const Token *tok, const char pattern[], unsigned int varId)
{
    for (; tok; tok = tok->next())
    {
        if (Token::Match(tok, pattern, varId))
            return tok;
    }
    return 0;
}

unsigned int Token::varId() const
{
    return _varId;
}

void Token::varId(unsigned int id)
{
    _varId = id;
}

Token *Token::next() const
{
    return _next;
}

void Token::next(Token *next)
{
    _next = next;
}

Token *Token::previous() const
{
    return _previous;
}

void Token::previous(Token *previous)
{
    _previous = previous;
}

void Token::insertToken(const char str[])
{
    Token *newToken = new Token;
    newToken->str(str);
    newToken->_linenr = _linenr;
    newToken->_fileIndex = _fileIndex;
    if (this->next())
    {
        newToken->next(this->next());
        newToken->next()->previous(newToken);
    }

    this->next(newToken);
    newToken->previous(this);
}

void Token::eraseTokens(Token *begin, const Token *end)
{
    if (! begin)
        return;

    while (begin->next() && begin->next() != end)
    {
        begin->deleteNext();
    }
}

unsigned int Token::fileIndex() const
{
    return _fileIndex;
}

void Token::fileIndex(unsigned int fileIndex)
{
    _fileIndex = fileIndex;
}

unsigned int Token::linenr() const
{
    return _linenr;
}

void Token::linenr(unsigned int linenr)
{
    _linenr = linenr;
}

void Token::link(Token *link)
{
    _link = link;
}

Token *Token::link() const
{
    return _link;
}

void Token::printOut(const char *title) const
{
    std::cout << stringifyList(true, title) << std::endl;
}

std::string Token::stringifyList(const bool varid, const char *title) const
{
    std::ostringstream ret;
    if (title)
        ret << "\n### " << title << " ###\n";

    unsigned int linenr = 0;
    int fileIndex = -1;
    std::map<unsigned int, unsigned int> lineNumbers;
    for (const Token *tok = this; tok; tok = tok->next())
    {
        bool fileChange = false;
        if (static_cast<int>(tok->_fileIndex) != fileIndex)
        {
            if (fileIndex != -1)
            {
                lineNumbers[fileIndex] = tok->_fileIndex;
            }

            fileIndex = static_cast<int>(tok->_fileIndex);
            ret << "\n\n##file " << fileIndex << "";

            linenr = lineNumbers[fileIndex];
            fileChange = true;
        }

        if (linenr != tok->linenr() || fileChange)
        {
            while (linenr < tok->linenr())
            {
                ++linenr;
                ret << "\n" << linenr << ":";
            }
            linenr = tok->linenr();
        }

        ret << " " << tok->str();
        if (varid && tok->varId() > 0)
            ret << "@" << tok->varId();
    }
    ret << "\n";
    return ret.str();
}



