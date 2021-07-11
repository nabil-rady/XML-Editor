#include "check.hpp"
#include <QDebug>



bool check(QString file)
{

	QStack<QString> checking;
	QStack<QString> closing;

	
	
	int len=file.length();
	for (int i = 0; i < len; i++)
	{
		if (file[i]=='<')
		{
			if (file[i + 1] != '/')
			{
				int start = i + 1;
				while (file[i] != '>')
				{
					i++;

				}
				int end = i;
//                QString tag = file.substr(start, end - start);
                QString tag = file.mid(start, end - start);
//                qDebug() << tag;
				checking.push(tag);

			}
			else
			{
				int start = i + 2;
				while (file[i] != '>')
				{
					i++;

				}
				int end = i;
//                QString tag = file.subString(start, end - start);
                QString tag = file.mid(start, end - start);
//                qDebug() << tag;
				if (!checking.empty()&&checking.top()==tag)
				{
					checking.pop();

				}
				else
				{
					closing.push(tag);
					
					return false;

				}

			}
			


		}

	}
	if (closing.empty()&&checking.empty())
	{
		return true;

	}
	else
	{
		
		return false;
	}

		


}


