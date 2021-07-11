#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSessionManager>
#include <QPlainTextEdit>
#include <QString>
#include <QGuiApplication>

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
    void on_actionOpen_XML_File_triggered();

    bool save();
    bool saveAs();

    void on_actionSave_triggered();

    void on_actionConvert_To_JSON_triggered();

    void on_actionBeautify_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

private:
    Ui::MainWindow *ui;
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QPlainTextEdit *textEdit;
    QString curFile;
};
#endif // MAINWINDOW_H
