#include "check.hpp"
#include <QDebug>
bool check(QString file,int *startindex,int *endindex)
{
    *startindex=0;
    *endindex=0;
	QStack<QString> checking;
	QStack<QString> closing;
    QStack<int> starting;
    QStack<int> ending;
	int len=file.length();
	for (int i = 0; i < len; i++)
	{
		if (file[i]=='<')
		{
            if (file[i + 1] == '?')
            {
                while (file[i]!='>')
                {i++;}
                continue;
            }
            if (file[i + 1] == '!'&&file[i + 2] == '-')
            {
                while(file[i] != '>')
                    i++;
                continue;
            }
			if (file[i + 1] != '/')
			{

				int start = i + 1;
                while (file[i] != '>'&& file[i] != ' ')
				{
                    i++;
                    if (!(i < len))
                                       {
                                           *startindex = start - 1;
                                           *endindex = len - 1;
                                           return false;
                                       }
				}
                if(file[i]==' ')
                {
                    int end = i;
                    QString tag = file.mid(start, end - start);
                    checking.push(tag);
                    starting.push(start);
                    ending.push(end);
                    while (file[i]!='>') {
                        i++;
                        if (!(i < len))
                                           {
                                               *startindex = start - 1;
                                               *endindex = end+1;
                                               return false;
                                           }
                    }
                    if(file[i-1]=='/')
                       { checking.pop();
                        starting.pop();
                        ending.pop();
                    }
                }
                else
                {
				int end = i;
                QString tag = file.mid(start, end - start);
				checking.push(tag);
                starting.push(start);
                ending.push(end);
                if(file[i-1]=='/')
                {checking.pop();
                    starting.pop();
                    ending.pop();
                }
                }
			}
			else
			{
				int start = i + 2;
				while (file[i] != '>')
				{
					i++;
                    if (!(i < len))
                                       {
                                           *startindex = start - 2;
                                           *endindex = len - 1;
                                           return false;
                                       }

				}
				int end = i;
                QString tag = file.mid(start, end - start);
				if (!checking.empty()&&checking.top()==tag)
				{
					checking.pop();
                    starting.pop();
                    ending.pop();
				}
				else
				{
					closing.push(tag);
                    if (*startindex==0)
                                       {
                                           *startindex = start-2;
                                           *endindex = end+1;
                                       }
					return false;
				}
			}
		}

	}
    if (closing.empty() && checking.empty())
       {
           return true;
       }
       else
       {
           if (closing.empty())
           {
               *startindex = starting.top()-1;
                   *endindex = ending.top()+1;
                   return false;
           }
           else
           {
               return false;
           }
    }

}
