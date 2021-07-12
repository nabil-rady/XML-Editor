#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QSaveFile>
#include <QSettings>
#include <QPlainTextEdit>
#include "lib/Graph.hpp"
#include "lib/check.hpp"
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

QFile XMLtemp("out.txt");
QFile XMLfile("myfile.txt");
QString fileloc="";
std::vector <std::string> lines;
void MainWindow::on_actionOpen_XML_File_triggered()
{
    QFile file(QFileDialog::getOpenFileName(this, tr("Open File"), QString(),tr("Text Files (*.xml)")));
    fileloc=file.fileName();
    //qDebug()<<fileloc;
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this,"..","can not open the file");
        return ;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
    std::string line;
    lines.resize(0);
    while (!file.atEnd())
    { line = (file.readLine().trimmed()).toStdString();
      lines.push_back(line);
    }
}
void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
             tr("Save Address Book"), "",
             tr("Address Book (*.xml);;Address Book (*.json);;All Files (*)"));
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly|QFile::Text))
    {
        QMessageBox::warning(this,"..","can not open the file");
        return ;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out<<text ;
    file.flush();
    file.close();

}








bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}
bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("*.xml") ;
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}
bool MainWindow::maybeSave()
{
    if (!textEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        //return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}
bool MainWindow::saveFile(const QString &fileName)
{
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << textEdit->toPlainText();
        if (!file.commit()) {
            errorMessage = tr("Cannot write file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    } else {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                       .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("Application"), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}
void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}
void MainWindow::on_actionConvert_To_JSON_triggered()
{
    ui->textEdit->clear();
    //QString text= textEdit->toPlainText();
    QFile file(fileloc);
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this,"..","can not open the file");
        return ;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    //ui->textEdit->setText(text);
    //QTextStream text(&file);
    //QString textfile = text.readAll();
    int start;
    int end;

    if (check(text,&start,&end))
    {
    Graph t = build_tree(text);
    QString Json_Output=t.convert_to_json();
    ui->textEdit->setText(Json_Output);
    }
    else
        QMessageBox::warning(this,"..","the xml is not consistent");



}


void MainWindow::on_actionBeautify_triggered()
{
    ui->textEdit->clear();
    //QString text= textEdit->toPlainText();
    QFile file(fileloc);
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this,"..","can not open the file");
        return ;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    int start;
    int end;

    if(check(text,&start,&end))
    {
    //ui->textEdit->setText(text);
    //QTextStream text(&file);
    //QString textfile = text.readAll();
    Graph t = build_tree(text);
    QString Beautify_Output=t.beautify_xml();
    ui->textEdit->setText(Beautify_Output);
    }
    else
        QMessageBox::warning(this,"..","Your xml is not consistent");
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionCheck_Consistency_triggered()
{
    QFile file(fileloc);
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this,"..","can not open the file");
        return ;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    int start=0,end=text.length()-1;
    QTextCharFormat format;
    QTextCursor cursor( ui->textEdit->textCursor() );
    std::string line;
    if(check(text,&start,&end))
    {
     QMessageBox enteredString;
     enteredString.setText("Correct XML File");
     enteredString.exec();
    }
    else
    {int j =0;
      for (int i=1;i<text.size()+1;i++)
      {
          //line=lines[i-1];
          if(i == start)
          {
              format.setFontWeight( QFont::TypeWriter );
              format.setForeground( QBrush( QColor(Qt::red) ) );
              cursor.setCharFormat( format );
              cursor.insertText(QString::fromStdString(line));
              if(cursor.PreviousCharacter != '\n'){cursor.insertText("\n");}
              //j++;
          }
          else
          {
              format.setFontWeight( QFont::TypeWriter );
              format.setForeground( QBrush( QColor(Qt::black) ) );
              cursor.setCharFormat( format );
              cursor.insertText(QString::fromStdString(line));
              if(cursor.PreviousCharacter != '\n'){cursor.insertText("\n");}
          }
      }
    }
//    if (check(text,&start,&end))
//    {
//        QMessageBox::information(this,"..","No errors found");

//    }
//    else
//    {
//        QString test =text.mid(start, end - start);
//        test.setTextColor("QLineEdit { background-color: yellow }");
//    }


}


void MainWindow::on_actionSolve_Errors_triggered()
{

}


void MainWindow::on_actionCopress_XML_File_triggered()
{
    QFile file(fileloc);
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this,"..","can not open the file");
        return ;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    QByteArray compressed=compress(text);
    QString fileName = QFileDialog::getSaveFileName(this,
             tr("Save Address Book"), "",
             tr("Address Book (*.zxml);;All Files (*)"));
    //QFile newDoc("C:/Users/HP/Desktop");
    QFile newDoc(fileName);
    if(newDoc.open(QIODevice::WriteOnly)){
        newDoc.write(compressed);
    }

    newDoc.close();
}


void MainWindow::on_actionDecompress_triggered()
{
    QFile file(QFileDialog::getOpenFileName(this, tr("Open File"), QString(),tr("Text Files (*.zxml)")));
    //QFile file("C:\\Users\\s\\Desktop\\bom1.xlsx");
        char file_data;
        QByteArray arr;
        if(!file.open(QFile::ReadOnly))
        {
            qDebug() << " Could not open the file for reading";
            return;
        }

        while(!file.atEnd())
        {
          // return value from 'file.read' should always be sizeof(char).
          file.read(&file_data,sizeof(char));
          arr.push_back(file_data);
          // do something with 'file_data'.

        }
        file.close();
//    QTextStream in(&file);
//    QByteArray text = in.readAll().toUtf8();
    //QString filelocation=file.fileName();
    QString txt=decompress(arr);
    ui->textEdit->clear();
    ui->textEdit->setText(txt);

}

