#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSessionManager>
#include <QPlainTextEdit>
#include <QString>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadFile(const QString &fileName);
private slots:
    void on_actionQuit_triggered();
    void newFile();
//    void open();
    bool save();
    bool saveAs();
//    void about();
//    void documentWasModified();
//    #ifndef QT_NO_SESSIONMANAGER
//        void commitData(QSessionManager &);
//    #endif
    void on_actionNew_XML_File_triggered();

    void on_actionOpen_XML_File_triggered();

private:
    Ui::MainWindow *ui;
//    void createActions();
//    void createStatusBar();
//    void readSettings();
//    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
//    QString strippedName(const QString &fullFileName);

    QPlainTextEdit *textEdit;
    QString curFile;
};
#endif // MAINWINDOW_H
