#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QSaveFile>
#include <QSettings>
#include <QPlainTextEdit>
#include "lib/Graph.hpp"
#include "lib/check.hpp"
#include "lib/fix_xml.hpp"
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
std::vector <std::string> lines;
void MainWindow::on_actionOpen_XML_File_triggered()
{
    QFile file(QFileDialog::getOpenFileName(this, tr("Open File"), QString(),tr("Text Files (*.xml)")));
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this,"..","can not open the file");
        return ;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
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
void MainWindow::on_actionConvert_To_JSON_triggered()
{
    int start;
    int end;

    if (check(ui->textEdit->toPlainText(),&start,&end))
    {
    Graph t = build_tree(ui->textEdit->toPlainText());
    QString Json_Output=t.convert_to_json();
    ui->textEdit->setText(Json_Output);
    }
    else
        QMessageBox::warning(this,"..","the xml is not consistent");
}


void MainWindow::on_actionBeautify_triggered()
{
    int start;
    int end;
    if (ui->textEdit->toPlainText()[0]=='{')
        QMessageBox::warning(this,"..","Error Cannot beautify jason file");
    else
    {
    if (check(ui->textEdit->toPlainText(),&start,&end))
    {
    Graph t = build_tree(ui->textEdit->toPlainText());
    QString beautify_Output=t.beautify_xml();
    ui->textEdit->setText(beautify_Output);
    }
    else
        QMessageBox::warning(this,"..","the xml is not consistent");
    }
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

    QString text = ui->textEdit->toPlainText();
    int startindex;
    int endindex;

    if (check(text,&startindex,&endindex))
    {
        QMessageBox::information(this,"..","Your Xml is consistent");


    }
    ui->textEdit->clear();
    int start=0,end=text.length()-1;
    QString before_error="";
    QString after_error="";
    QString error="";
    QString notmodified="";
    check(text,&start,&end);
    for (int i=0;i<text.length();i++)
    {
        if (i<start)
        {
            before_error+=text[i];
        }
        if (i>=start&&i<=end-1)
        {
            error+=text[i];
        }
        if (i>end-1)
            after_error+=text[i];
    }
    ui->textEdit->setTextColor( QColor( "black" ) );
    ui->textEdit->append(before_error);
    ui->textEdit->setTextColor( QColor( "red" ) );
    ui->textEdit->append(error);
    if (end !=text.length()-1)
    {
        ui->textEdit->setTextColor( QColor( "black" ) );
        ui->textEdit->append(after_error);
    }
}


void MainWindow::on_actionSolve_Errors_triggered()
{
    QString new_xml=fix_xml(ui->textEdit->toPlainText());
    if(new_xml=="")
    {
        QMessageBox::information(this,"..","Your Xml has no errors");


    }
    else
    {
        ui->textEdit->setText(new_xml);
        QMessageBox::information(this,"..","One bug solved");
    }
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
    QFile newDoc(fileName);
    if(newDoc.open(QIODevice::WriteOnly)){
        newDoc.write(compressed);
    }

    newDoc.close();
}


void MainWindow::on_actionDecompress_triggered()
{
    QFile file(QFileDialog::getOpenFileName(this, tr("Open File"), QString(),tr("Text Files (*.zxml)")));
        char file_data;
        QByteArray arr;
        if(!file.open(QFile::ReadOnly))
        {
            QMessageBox::warning(this,"..","can not open the file");
            return;
        }

        while(!file.atEnd())
        {
          file.read(&file_data,sizeof(char));
          arr.push_back(file_data);
        }
        file.close();
    QString txt = decompress(arr);
    ui->textEdit->clear();
    ui->textEdit->setText(txt);
}

void MainWindow::on_actionMinify_triggered()
{
    int start;
    int end;

    if (check(ui->textEdit->toPlainText(),&start,&end))
    {
    //Graph t = build_tree(ui->textEdit->toPlainText());
    QString minify_Output=minify(ui->textEdit->toPlainText());
    ui->textEdit->setText(minify_Output);
    }
    else
        QMessageBox::warning(this,"..","The xml file is not consistent");
}

