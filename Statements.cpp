
#include "Statements.h"
#include "tokenize.h"

#include "CommonCheck.h"    // <- IsName

#include <iostream>
#include <sstream>

std::vector<std::string> VariableNames;
std::list<STATEMENT> Statements;

extern bool Debug;




//---------------------------------------------------------------------------
// Create statement list
//---------------------------------------------------------------------------

void AppendStatement(STATEMENT::etype Type, TOKEN *tok, std::string Var="")
{
    STATEMENT NewStatement;
    NewStatement.Type = Type;
    NewStatement.Token = tok;
    if (Var.empty())
    {
        NewStatement.VarIndex = 0;
    }
    else
    {
        bool Found = false;
        for (unsigned int i = 0; i < VariableNames.size(); i++)
        {
            if (VariableNames[i] == Var)
            {
                Found = true;
                NewStatement.VarIndex = i;
                break;
            }
        }

        if ( ! Found )
        {
            NewStatement.VarIndex = VariableNames.size();
            VariableNames.push_back(Var);
        }
    }
    Statements.push_back(NewStatement);
}

TOKEN *GotoNextStatement(TOKEN *tok)
{
    if (tok && (tok->str[0]=='{' || tok->str[0]=='}'))
        return tok->next;

    if (tok)
        tok = tok->next;
    int parlevel = 0;
    for (; tok; tok = tok->next)
    {
        if (tok->str[0] == '(')
            parlevel++;
        else if (tok->str[0] == ')')
            parlevel--;
        if (strchr("{}", tok->str[0]))
            break;
        if (parlevel==0 && tok->str[0] == ';')
        {
            while (tok && tok->str[0] == ';')
                tok = tok->next;
            break;
        }
    }
    return tok;
}


void GetVariableName(TOKEN * &Token, std::string &varname)
{
    varname = "";

    if (Token == NULL)
        return;

    if (!IsName(Token->str))
        return;

    // Get variable name..
    std::ostringstream ostr;
    bool array = false;
    bool name  = false;
    for (TOKEN *tok = Token; tok; tok = tok->next)
    {
        const char *str = tok->str;

        if ((array) || (str[0]=='['))
        {
            ostr << str;
            array = (str[0] != ']');
        }

        else
        {
            if (name && IsName(str))
                return;
            name = IsName(str);
            if (!name && strcmp(str,".") && strcmp(str,"->") && strcmp(str,"::"))
            {
                if (str[0] == '(')
                    return;
                Token = tok;
                break;
            }
            ostr << str;
        }
    }

    varname = ostr.str();
}


void CreateStatementList()
{
    // Clear lists..
    VariableNames.clear();
    Statements.clear();

    int indentlevel = 0;
    for (TOKEN *tok = tokens; tok; tok = GotoNextStatement(tok))
    {
        if (tok->str[0] == '{')
        {
            AppendStatement(STATEMENT::OBRACE, tok);
            indentlevel++;
        }
        else if (tok->str[0] == '}')
        {
            AppendStatement(STATEMENT::EBRACE, tok);
            indentlevel--;
        }
        else if (indentlevel >= 1)
        {
            bool hasif = false, hasloop = false, hasswitch = false;

            if (strcmp(tok->str,"if")==0)
            {
                hasif = true;
                AppendStatement(STATEMENT::IF, tok);
            }
            else if (strcmp(tok->str,"else")==0)
            {
                hasif = true;
                if (strcmp(getstr(tok,1),"if")==0)
                    AppendStatement(STATEMENT::ELSEIF, tok);
                else
                    AppendStatement(STATEMENT::ELSE, tok);
            }

            else if (strcmp(tok->str,"do")==0 ||
                     strcmp(tok->str,"while")==0 ||
                     strcmp(tok->str,"for")==0)
            {
                AppendStatement(STATEMENT::LOOP, tok);
                hasloop = true;
            }

            else if (strcmp(tok->str,"switch")==0)
            {
                AppendStatement(STATEMENT::SWITCH, tok);
                hasswitch = true;
            }

            // Declaring variables..
            if (IsName(tok->str) && strcmp(tok->str,"delete") && strcmp(tok->str,"return"))
            {
                const char *str1 = getstr(tok, 1);
                bool decl = IsName(str1) || str1[0]=='*';
                for (TOKEN *tok2 = decl ? tok->next : NULL; tok2; tok2 = tok2->next)
                {
                    if (strchr("{};.", tok2->str[0]))
                        break;

                    const char *str1 = getstr(tok2, 1);
                    if (IsName(tok2->str) && strchr("[=,;",str1[0]))
                    {
                        AppendStatement(STATEMENT::DECL, tok2, tok2->str);
                        while (tok2 && !strchr(",;", tok2->str[0]))
                            tok2 = tok2->next;
                        if (tok2->str[0] == ';')
                            break;
                    }
                }
            }


            // Assign..
            for (TOKEN *tok2 = tok; tok2; tok2 = tok2->next)
            {
                if (strchr("{};", tok2->str[0]))
                    break;

                TOKEN *eq = tok2;
                std::string varname = "";
                GetVariableName(eq, varname);

                // Equal with..
                if (eq && strcmp(eq->str,"=")==0 && !varname.empty())
                {
                    TOKEN *rs = eq->next;

                    bool ismalloc = false;
                    ismalloc |= match(rs, "strdup (");
                    if (rs->str[0]=='(' && IsName(getstr(rs,1)))
                    {
                        ismalloc |= match(rs, "( type * ) malloc (");
                        ismalloc |= match(rs, "( type * * ) malloc (");
                        ismalloc |= match(rs, "( type type * ) malloc (");
                        ismalloc |= match(rs, "( type type * * ) malloc (");
                    }

                    if ( ismalloc )
                        AppendStatement(STATEMENT::MALLOC, tok2, varname);

                    else if ( match(rs,"new type ;") )
                        AppendStatement(STATEMENT::NEW, tok2, varname);

                    else if ( match(rs, "new type (") )
                        AppendStatement(STATEMENT::NEW, tok2, varname);

                    else if ( match(rs, "new type [") )
                        AppendStatement(STATEMENT::NEWARRAY, tok2, varname);

                    else
                        AppendStatement(STATEMENT::ASSIGN, tok2, varname);

                    tok2 = eq;
                }
            }

            // Delete..
            for (TOKEN *tok2 = tok; tok2; tok2 = tok2->next)
            {
                if (strchr("{};", tok2->str[0]))
                    break;

                if (match(tok2, "free ( var ) ;"))
                    AppendStatement(STATEMENT::FREE, tok2, getstr(tok2, 2));

                if (match(tok2, "delete var ;"))
                    AppendStatement(STATEMENT::DELETE, tok2, getstr(tok2,1));

                if (match(tok2, "delete [ ] var ;"))
                    AppendStatement(STATEMENT::DELETEARRAY, tok2, getstr(tok2,3));
            }

            // Use..
            bool UseVar = false;
            int parlevel = 0;
            for (TOKEN *tok2 = tok; tok2; tok2 = tok2->next)
            {
                if (parlevel==0 && strchr("{};", tok2->str[0]))
                    break;

                if (tok2->str[0] == '(')
                    parlevel++;
                if (tok2->str[0] == ')')
                    parlevel--;
                if (parlevel == 0 && tok2->str[0] == ',')
                    UseVar = false;

                else if (tok2->str[0]=='=' || tok2->str[0]=='(')
                    UseVar = true;

                else if (UseVar && IsName(tok2->str))
                {
                    std::string varname = "";
                    GetVariableName(tok2, varname);

                    if (!varname.empty() &&
                        varname!="continue" &&
                        varname!="break" &&
                        varname!="return")
                    {
                        AppendStatement(STATEMENT::USE, tok2, varname);
                    }

                    if (tok2->str[0] == ')')
                        parlevel--;

                    if (parlevel==0 && tok2->str[0]==';')
                        break;
                }
            }

            // Return, continue, break..
            for (TOKEN *tok2 = tok; tok2; tok2 = tok2->next)
            {
                if (strchr("{};", tok2->str[0]))
                    break;

                if (strcmp(tok2->str,"continue")==0)
                {
                    AppendStatement(STATEMENT::CONTINUE, tok2);
                    break;
                }

                if (strcmp(tok2->str,"break")==0)
                {
                    AppendStatement(STATEMENT::BREAK, tok2);
                    break;
                }

                if (strcmp(tok2->str,"return")==0)
                {
                    if (IsName(getstr(tok2,1)) && strcmp(getstr(tok2,2),";")==0)
                        AppendStatement(STATEMENT::RETURN, tok2, getstr(tok2,1));

                    else
                        AppendStatement(STATEMENT::RETURN, tok2, ";");

                    break;
                }
            }

            if (hasif)
            {
                AppendStatement(STATEMENT::ENDIF, tok);
            }
            else if (hasloop)
            {
                AppendStatement(STATEMENT::ENDLOOP, tok);
            }
            else if (hasswitch)
            {
                AppendStatement(STATEMENT::ENDSWITCH, tok);
            }
        }
    }

    if (Debug)
    {
        std::list<STATEMENT>::const_iterator it;
        for (it = Statements.begin(); it != Statements.end(); it++)
        {
            STATEMENT s = *it;
            std::cout << it->Token->linenr << " : ";
            switch (s.Type)
            {
                case STATEMENT::OBRACE:
                    std::cout << "{";
                    break;

                case STATEMENT::EBRACE:
                    std::cout << "}";
                    break;

                case STATEMENT::DECL:
                    std::cout << "decl " << VariableNames[s.VarIndex];
                    break;

                case STATEMENT::ASSIGN:
                    std::cout << "assign " << VariableNames[s.VarIndex];
                    break;

                case STATEMENT::MALLOC:
                    std::cout << "malloc " << VariableNames[s.VarIndex];
                    break;

                case STATEMENT::FREE:
                    std::cout << "free " << VariableNames[s.VarIndex];
                    break;

                case STATEMENT::NEW:
                    std::cout << "new " << VariableNames[s.VarIndex];
                    break;

                case STATEMENT::NEWARRAY:
                    std::cout << "new[] " << VariableNames[s.VarIndex];
                    break;

                case STATEMENT::DELETE:
                    std::cout << "delete " << VariableNames[s.VarIndex];
                    break;

                case STATEMENT::DELETEARRAY:
                    std::cout << "delete[] " << VariableNames[s.VarIndex];
                    break;

                case STATEMENT::USE:
                    std::cout << "use " << VariableNames[s.VarIndex];
                    break;


                case STATEMENT::LOOP:
                    std::cout << "loop";
                    break;

                case STATEMENT::ENDLOOP:
                    std::cout << "endloop";
                    break;


                case STATEMENT::SWITCH:
                    std::cout << "switch";
                    break;

                case STATEMENT::ENDSWITCH:
                    std::cout << "endswitch";
                    break;


                case STATEMENT::IF:
                    std::cout << "if";
                    break;

                case STATEMENT::ELSEIF:
                    std::cout << "elseif";
                    break;

                case STATEMENT::ELSE:
                    std::cout << "else";
                    break;

                case STATEMENT::ENDIF:
                    std::cout << "endif";
                    break;

                case STATEMENT::RETURN:
                    std::cout << "return " << VariableNames[s.VarIndex];
                    break;

                case STATEMENT::CONTINUE:
                    std::cout << "continue";
                    break;

                case STATEMENT::BREAK:
                    std::cout << "break";
                    break;

                default:
                    std::cout << "ERROR. Unknown code!!";
                    break;
            }
            std::cout << "\n";
        }
    }
}
//---------------------------------------------------------------------------


