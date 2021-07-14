#include "fix_xml.hpp"
#include "QMessageBox"

QString fix_xml(QString text){
    int startindex, endindex, openingtags, closingtags, selfclosingtags;
        QString expected;
       if( check_with_counting(text, &startindex, &endindex, &openingtags, &closingtags, &selfclosingtags,&expected))
           return"";
       if (openingtags>closingtags)
        {
           if (startindex==0)
           {
               return text + "</" + text.mid(startindex + 1, endindex - startindex-2) + ">";

           }


            int index = startindex;
            while (1)
            {
                while (text[index] != '>')
                {
                    index--;

                }
                int maybeclosingtag = index - 1;
                while (text[index] != '<')
                {
                    if (text[index]==' ')
                    {
                         maybeclosingtag = index - 1;

                    }
                    index--;
                }
                int maybeopeningtag = index + 1;


                QString maybetag = text.mid(maybeopeningtag,( maybeclosingtag-maybeopeningtag)+1);
                if (maybetag==expected)
                {
                    index++;
                    while (text[index] != '<')
                    {

                        index++;

                    }
                    return text.mid(0, index) + "</" + expected + ">" + text.mid(index, text.length() - index);



                }
            }

        }

       if (closingtags>openingtags)
         {
             return text.mid(0, startindex)+ text.mid(endindex, text.length() - endindex);
         }
         if (closingtags==openingtags)
         {
             return text.mid(0, startindex ) + "</" + expected + ">" + text.mid(endindex , text.length() - endindex);
         }
}
bool check_with_counting(QString file, int* startindex, int* endindex, int* openingtag, int* closingtag, int* selfclosingtag, QString* expected)
{
    *startindex = 0;
    *endindex = 0;
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
                    if (!checking.empty())
                    {
                        *expected = checking.top();


                    }


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


