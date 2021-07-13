#include "fix_xml.hpp"

QString fix_xml(QString xml){
//    if (check(xml)){
//        return xml;
//    }

    QString ans = "";
    long long len = xml.length();
    for (long long i = 0; i < len; i++){

    }
    return ans;
}






bool check_with_counting(QString file, int* startindex, int* endindex, int* openingtag, int* closingtag, int* selfclosingtag)
{
    *openingtag =0 ;
    *closingtag =0 ;
    *selfclosingtag = 0;

    QStack<QString> checking;
    QStack<QString> closing;
    QStack<int> starting;

    QStack<int> ending;





    int len = file.length();
    for (int i = 0; i < len; i++)
    {
        if (file[i] == '<')
        {
            if (file[i + 1] == '?')
            {
                while (file[i] != '>')
                {
                    i++;

                }
                continue;

            }
            if (file[i + 1] == '!' && file[i + 2] == '-')
            {
                while (file[i] != '>')
                    i++;
                continue;



            }
            if (file[i + 1] != '/')
            {

                int start = i + 1;
                while (file[i] != '>' && file[i] != ' ')
                {
                    i++;


                }
                if (file[i] == ' ')
                {
                    int end = i;
                    //                QString tag = file.substr(start, end - start);
                   QString tag = file.mid(start, end - start);
                    //                qDebug() << tag;
                    checking.push(tag);
                    starting.push(start);
                    ending.push(end);
                    *openingtag+=1;
                    while (file[i] != '>') {
                        i++;

                    }
                    if (file[i - 1] == '/')
                    {
                        *openingtag-=1;
                        *selfclosingtag+=1;
                        checking.pop();
                        starting.pop();
                        ending.pop();
                    }



                }
                else
                {

                    int end = i;
                    //                QString tag = file.substr(start, end - start);
                    QString tag = file.mid(start, end - start);
                    //                qDebug() << tag;
                    checking.push(tag);
                    starting.push(start);
                    ending.push(end);
                    *openingtag+=1;
                    if (file[i - 1] == '/')
                    {
                        *openingtag-=1;
                        *selfclosingtag+=1;
                        checking.pop();
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
///////////////////////////////////////////////////////////////////////////////////////////////////////
                }
                *closingtag+=1;
                int end = i;
                //                QString tag = file.subString(start, end - start);
                QString tag = file.mid(start, end - start);
                //                qDebug() << tag;
                if (!checking.empty() && checking.top() == tag)
                {
                    //qDebug().noquote() << checking.top();
                    checking.pop();
                    starting.pop();
                    ending.pop();

                }
                else
                {
                    closing.push(tag);

                    if (*startindex == 0)
                    {
                        *startindex = start - 2;
                        *endindex = end + 1;

                    }


                    // return false;

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
            *startindex = starting.top() - 1;
            *endindex = ending.top() + 1;


            return false;

        }
        else
        {

            return false;
        }

    }




}

