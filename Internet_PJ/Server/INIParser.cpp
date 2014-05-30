#include "include/INIParser.h"

/*
 * char* GetInitKey(FileName, Section, Key)
 * Return Key=>Value
 * Ex:
 *
 * config.ini
 * [config]
 * dbhost=localhost
 *
 * Usage:
 * char dbhost[20];
 * strcpy(dbhost,GetInitKey("config.ini", "config", "dbhost"));
 */

char * INIParser::rstrip(char *word)
{
    int len = 0;
    int i = 0;
    int badnum = 0;
    char *p = NULL;
    assert(word!=NULL);
    len = strlen(word);
    p = word+len;
    for(p = word+len-1;p>=word;p--)
    {
        if(strchr(" \t;#", *(p)) > 0 )
        {
            badnum++; 
            if(*p == ';' || *p == '#')
            {
                //*p = 0;
            }  
            *p = 0;
        }
    }
    len = strlen(word);
    p = (char*)malloc(len+1);
    memset(p,0,len+1);
    strncpy(p,word,len);
    return p;
}


char * INIParser::GetInitKey(const char *filename, const char *title, const char *key)
{
    FILE *fp;
    char tmpLine[MAX_BUFSIZE] = {0};
    char tmpstr[MAX_BUFSIZE] = {0};
    char *tmp = NULL;
    int rtnval;
    int i = 0;
    int flag = 0;

    char *retstr = NULL;

    memset(tmpLine,0,sizeof(tmpLine));
    memset(tmpstr,0,sizeof(tmpstr));
 
    if ((fp = fopen(filename, "r")) == NULL) 
    {
        cout<<"[DEBUG] file: "<<filename<<" don't exits"<<endl;
        assert(fp>0);
        return NULL;
    }

    while (!feof(fp)) 
    {

        rtnval = fgetc(fp);
        if (rtnval == EOF) 
        {
            tmpLine[i++] = 0;
        } 
        else 
        {
            tmpLine[i++] = rtnval;
        }
        if (rtnval == '\n' || rtnval == EOF) 
        {
            tmpLine[--i] = 0;
            i = 0;
            if (tmpLine[0] == 0 || strchr("#;", tmpLine[0]) != NULL)
            {
                continue;    // Skip null line or comment line
            }
            if (tmpLine[0] == '[') 
            {
                tmp = NULL;
                if (flag == 1) 
                {
                    // Meet next section
//#ifdef PROCESS_DUP_SECTION    // INI file exist duplicate section
                    flag = 0;
//#else
                    break;    // Skip other context
//#endif
                }
            } 
            else 
            {
                tmp = strchr(tmpLine, '=');
            }

            if ((tmp != NULL) && (flag == 1)) 
            {
                char *p;

                *tmp = 0;    // erase '=', spilte Key and Value

                p = strstr(tmpLine, key);
                if (p != NULL) 
                {
                    if (p > tmpLine && strchr(" \t", *(p - 1)) == NULL)
                    {
                        continue;    // exist prefix, mishit key
                    }
                    p += strlen(key);
                    if (*p == 0 || strchr(" \t", *p) != NULL) 
                    {
                        tmp++;    // Skip '='
                        while (*tmp == ' ' || *tmp == '\t')
                        {
                            tmp++;    // Skip left lead ' ' or '\t'
                        }
                        tmp = rstrip(tmp);
                        fclose(fp);
                        return tmp;
                    }
                }
            } 
            else 
            {
                strcpy(tmpstr, "[");
                strcat(tmpstr, title);
                strcat(tmpstr, "]");
                if (strcmp(tmpstr, tmpLine) == 0) 
                {
                    flag = 1;
                }
            }

        }

        if (rtnval == EOF) 
        {
            break;
        }
    
    }   

    // end of while
    fclose(fp);
    retstr = (char*)"";
    return retstr;
}
