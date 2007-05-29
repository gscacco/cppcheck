//---------------------------------------------------------------------------
#include "CheckClass.h"
#include "Tokenize.h"
#include "CommonCheck.h"
#include <locale>
#include <list>
#include <string>
#include <sstream>
//---------------------------------------------------------------------------

struct VAR
{
    bool        is_class;
    const char *name;
    bool        init;
    bool        is_pointer;
    struct VAR *next;
};
//---------------------------------------------------------------------------

static struct VAR *ClassChecking_GetVarList(const char classname[])
{
    // Locate class..
    const char *pattern[] = {"class","","{",0};
    pattern[1] = classname;
    TOKEN *tok1 = findtoken(tokens, pattern);

    // All classes..
    struct _class
    {
        const char *name;
        struct _class *next;
    };
    struct _class *classes = NULL;
    const char *pattern_anyclass[] = {"class","",NULL};
    for (TOKEN *t = findtoken(tokens,pattern_anyclass); t; t = findtoken(t->next,pattern_anyclass))
    {
        _class *newclass = new _class;
        newclass->name = t->next->str;
        newclass->next = classes;
        classes = newclass;
    }

    // Get variable list..
    bool is_class = false;
    bool is_pointer = false;
    struct VAR *varlist = NULL;
    unsigned int indentlevel = 0;
    for (TOKEN *tok = tok1; tok; tok = tok->next)
    {
        if (!tok->next)
            break;

        if (tok->str[0] == '{')
            indentlevel++;
        if (tok->str[0] == '}')
        {
            if (indentlevel <= 1)
                break;
            indentlevel--;
        }

        if (strchr(";{}", tok->str[0]))
            is_class = is_pointer = false;
        else if (IsName(tok->str))
        {
            for (_class *c = classes; c; c = c->next)
                is_class |= (strcmp(c->name, tok->str) == 0);
        }

        if (tok->str[0] == '*')
            is_pointer = true;

        // Member variable?
        if ((indentlevel == 1) &&
            (tok->next->str[0] == ';') &&
            (IsName(tok->str)) &&
            (strcmp(tok->str,"const") != 0 ))
        {
            struct VAR *var = new VAR;
            memset(var, 0, sizeof(struct VAR));
            var->name = tok->str;
            var->next = varlist;
            var->is_class = is_class;
            var->is_pointer = is_pointer;
            varlist   = var;
        }
    }

    while (classes)
    {
        _class *next = classes->next;
        delete classes;
        classes = next;
    }

    return varlist;
}
//---------------------------------------------------------------------------

static TOKEN * ClassChecking_VarList_RemoveAssigned(TOKEN *_tokens, struct VAR *varlist, const char classname[], const char funcname[])
{
    // Locate class member function
    const char *pattern[] = {"","::","","(",NULL};
    pattern[0] = classname;
    pattern[2] = funcname;

    // Locate member function implementation..
    TOKEN *ftok = findtoken(_tokens, pattern);
    if (!ftok)
        return NULL;

    bool BeginLine = false;
    bool Assign = false;
    unsigned int indentlevel = 0;
    for (; ftok; ftok = ftok->next)
    {
        if (!ftok->next)
            break;

        // Class constructor.. initializing variables like this
        // clKalle::clKalle() : var(value) { }
        if (indentlevel==0 && strcmp(classname,funcname)==0)
        {
            if (Assign &&
                IsName(ftok->str) &&
                ftok->next->str[0]=='(')
            {
                for (struct VAR *var = varlist; var; var = var->next)
                {
                    if (strcmp(var->name,ftok->str))
                        continue;
                    var->init = true;
                    break;
                }
            }

            Assign |= (ftok->str[0] == ':');
        }


        if (ftok->str[0] == '{')
        {
            indentlevel++;
            Assign = false;
        }

        if (ftok->str[0] == '}')
        {
            if (indentlevel <= 1)
                break;
            indentlevel--;
        }

        if (BeginLine && indentlevel>=1 && IsName(ftok->str))
        {
            // Clearing all variables..
            if (match(ftok,"memset ( this ,"))
            {
                for (struct VAR *var = varlist; var; var = var->next)
                    var->init = true;
            }

            // Calling member function?
            if (ftok->next->str[0] == '(')
                ClassChecking_VarList_RemoveAssigned(tokens, varlist, classname, ftok->str);

            // Assignment of member variable?
            if (strcmp(ftok->next->str, "=") == 0)
            {
                for (struct VAR *var = varlist; var; var = var->next)
                {
                    if (strcmp(var->name,ftok->str))
                        continue;
                    var->init = true;
                    break;
                }
            }

            // Calling member function..
            if (strcmp(ftok->next->str,".")==0 || strcmp(ftok->next->str,"->")==0)
            {
                // The functions 'clear' and 'Clear' are supposed to initialize variable.
                if (stricmp(ftok->next->next->str,"clear") == 0)
                {
                    for (struct VAR *var = varlist; var; var = var->next)
                    {
                        if (strcmp(var->name,ftok->str))
                            continue;
                        var->init = true;
                        break;
                    }
                }
            }
        }

        BeginLine = (strchr("{};", ftok->str[0]));
    }

    return ftok;
}






//---------------------------------------------------------------------------
// ClassCheck: Check that all class constructors are ok.
//---------------------------------------------------------------------------

void CheckConstructors()
{
    // Locate class
    const char *pattern_classname[] = {"class","","{",NULL};
    TOKEN *tok1 = findtoken(tokens, pattern_classname);
    while (tok1)
    {
        const char *classname = tok1->next->str;

        // Are there a class constructor?
        const char *pattern_constructor[] = {"clKalle","::","clKalle","(",NULL};
        pattern_constructor[0] = classname;
        pattern_constructor[2] = classname;
        if (!findtoken(tokens,pattern_constructor))
        {
            // There's no class implementation, it must be somewhere else.
            tok1 = findtoken( tok1->next, pattern_classname );
            continue;
        }

        // Check that all member variables are initialized..
        struct VAR *varlist = ClassChecking_GetVarList(classname);
        ClassChecking_VarList_RemoveAssigned(tokens, varlist, classname, classname);

        // Check if any variables are uninitialized
        for (struct VAR *var = varlist; var; var = var->next)
        {
            if (!var->init && (var->is_pointer || !var->is_class))
            {
                std::ostringstream ostr;
                ostr << "Uninitialized member variable '" << classname << "::" << var->name << "'";
                ReportErr(ostr.str());
            }
        }

        // Delete the varlist..
        while (varlist)
        {
            struct VAR *nextvar = varlist->next;
            delete varlist;
            varlist = nextvar;
        }

        tok1 = findtoken( tok1->next, pattern_classname );
    }
}



//---------------------------------------------------------------------------
// ClassCheck: Unused private functions
//---------------------------------------------------------------------------

void CheckUnusedPrivateFunctions()
{
    // Locate some class
    const char *pattern_class[] = {"class","","{",NULL};
    for (TOKEN *tok1 = findtoken(tokens, pattern_class); tok1; tok1 = findtoken(tok1->next, pattern_class))
    {
        const char *classname = tok1->next->str;

        // The class implementation must be available..
        const char *pattern_classconstructor[] = {"","::","",NULL};
        pattern_classconstructor[0] = classname;
        pattern_classconstructor[2] = classname;
        if (!findtoken(tokens,pattern_classconstructor))
            continue;

        // Get private functions..
        std::list<std::string> FuncList;
        FuncList.clear();
        bool priv = false;
        unsigned int indent_level = 0;
        for (TOKEN *tok = tok1; tok; tok = tok->next)
        {
            if (match(tok,"friend class"))
            {
                // Todo: Handle friend classes
                FuncList.clear();
                break;
            }

            if (tok->str[0] == '{')
                indent_level++;
            if (tok->str[0] == '}')
            {
                if (indent_level <= 1)
                    break;
                indent_level--;
            }
            if (strcmp(tok->str,"};") == 0)
                break;
            if (strcmp(tok->str,"private:") == 0)
                priv = true;
            else if (strcmp(tok->str,"public:") == 0)
                priv = false;
            else if (strcmp(tok->str,"protected:") == 0)
                priv = false;
            else if (priv && indent_level == 1)
            {
                if (std::isalpha(tok->str[0]) &&
                    tok->next->str[0]=='(' &&
                    strcmp(tok->str,classname) != 0)
                {
                    FuncList.push_back(tok->str);
                }
            }
        }

        // Check that all private functions are used..
        const char *pattern_function[] = {"","::",NULL};
        pattern_function[0] = classname;
        bool HasFuncImpl = false;
        for (TOKEN *ftok = findtoken(tokens, pattern_function); ftok; ftok = findtoken(ftok->next,pattern_function))
        {
            int numpar = 0;
            while (ftok && ftok->str[0]!=';' && ftok->str[0]!='{')
            {
                if (ftok->str[0] == '(')
                    numpar++;
                else if (ftok->str[0] == ')')
                    numpar--;
                ftok = ftok->next;
            }

            if (!ftok)
                break;

            if (ftok->str[0] == ';')
                continue;

            if (numpar != 0)
                continue;

            HasFuncImpl = true;

            indent_level = 0;
            while (ftok)
            {
                if (ftok->str[0] == '{')
                    indent_level++;
                if (ftok->str[0] == '}')
                {
                    if (indent_level<=1)
                        break;
                    indent_level--;
                }
                if (ftok->next && ftok->next->str[0] == '(')
                    FuncList.remove(ftok->str);
                ftok = ftok->next;
            }
        }

        while (HasFuncImpl && !FuncList.empty())
        {
            bool fp = false;

            // Final check; check if the function pointer is used somewhere..
            const char *_pattern[] = {"=","",NULL};
            _pattern[1] = FuncList.front().c_str();
            fp |= (findtoken(tokens, _pattern) != NULL);
            _pattern[0] = "(";
            fp |= (findtoken(tokens, _pattern) != NULL);
            _pattern[0] = ")";
            fp |= (findtoken(tokens, _pattern) != NULL);
            _pattern[0] = ",";
            fp |= (findtoken(tokens, _pattern) != NULL);

            if (!fp)
            {
                std::ostringstream ostr;
                ostr << "Class '" << classname << "', unused private function: '" << FuncList.front() << "'";
                ReportErr(ostr.str());
            }
            FuncList.pop_front();
        }
    }
}

//---------------------------------------------------------------------------
// ClassCheck: Check that memset is not used on classes
//---------------------------------------------------------------------------

void CheckMemset()
{
    // Locate all 'memset' tokens..
    for (TOKEN *tok = tokens; tok; tok = tok->next)
    {
        if (strcmp(tok->str,"memset")!=0)
            continue;

        const char *type = NULL;
        if (match(tok, "memset ( var , num , sizeof ( type ) )"))
            type = getstr(tok, 8);
        else if (match(tok, "memset ( & var , num , sizeof ( type ) )"))
            type = getstr(tok, 9);
        else if (match(tok, "memset ( var , num , sizeof ( struct type ) )"))
            type = getstr(tok, 9);
        else if (match(tok, "memset ( & var , num , sizeof ( struct type ) )"))
            type = getstr(tok, 10);

        // No type defined => The tokens didn't match
        if (!(type && type[0]))
            continue;

        // It will be assumed that memset can be used upon 'this'.
        // Todo: Check this too
        if (strcmp(getstr(tok,2),"this") == 0)
            continue;

        // Warn if type is a class..
        const char *pattern1[] = {"class","",NULL};
        pattern1[1] = type;
        if (findtoken(tokens,pattern1))
        {
            std::ostringstream ostr;
            ostr << FileLine(tok) << ": Using 'memset' on class.";
            ReportErr(ostr.str());
        }

        // Warn if type is a struct that contains any std::*
        const char *pattern2[] = {"struct","","{",NULL};
        pattern2[1] = type;
        for (TOKEN *tstruct = findtoken(tokens, pattern2); tstruct; tstruct = tstruct->next)
        {
            if (tstruct->str[0] == '}')
                break;

            if (match(tstruct, "std :: type var ;"))
            {
                std::ostringstream ostr;
                ostr << FileLine(tok) << ": Using 'memset' on struct that contains a 'std::" << getstr(tstruct,2) << "'";
                ReportErr(ostr.str());
                break;
            }
        }
    }
}




//---------------------------------------------------------------------------
// ClassCheck: "void operator=("
//---------------------------------------------------------------------------

void CheckOperatorEq1()
{
    const char *pattern[] = {"void", "operator", "=", "(", NULL};
    if (TOKEN *tok = findtoken(tokens,pattern))
    {
        std::ostringstream ostr;
        ostr << FileLine(tok) << ": 'operator=' should return something";
        ReportErr(ostr.str());
    }
}

